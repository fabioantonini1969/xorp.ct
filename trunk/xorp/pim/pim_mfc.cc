// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001,2002 International Computer Science Institute
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software")
// to deal in the Software without restriction, subject to the conditions
// listed in the XORP LICENSE file. These conditions include: you must
// preserve this copyright notice, and you cannot mention the copyright
// holders in advertising related to the Software without their permission.
// The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
// notice is a summary of the XORP LICENSE file; the license in that file is
// legally binding.

#ident "$XORP: xorp/pim/pim_mfc.cc,v 1.7 2003/02/06 01:39:19 pavlin Exp $"

//
// PIM Multicast Forwarding Cache handling
//


#include "pim_module.h"
#include "pim_private.hh"
#include "pim_mfc.hh"
#include "pim_node.hh"
#include "pim_vif.hh"


//
// Exported variables
//

//
// Local constants definitions
//

//
// Local structures/classes, typedefs and macros
//

//
// Local variables
//

//
// Local functions prototypes
//


PimMfc::PimMfc(PimMrt& pim_mrt, const IPvX& source, const IPvX& group)
    : Mre<PimMfc>(source, group),
    _pim_mrt(pim_mrt),
    _rp_addr(IPvX::ZERO(family())),
    _iif_vif_index(Vif::VIF_INDEX_INVALID),
    _flags(0)
{
    
}

PimMfc::~PimMfc()
{
    // TODO: should we always call delete_mfc_from_kernel()?
    delete_mfc_from_kernel();
    
    //
    // Remove this entry from the RP table.
    //
    pim_node().rp_table().delete_pim_mfc(this);
    
    //
    // Remove this entry from the PimMrt table.
    //
    pim_mrt().remove_pim_mfc(this);
}

PimNode&
PimMfc::pim_node() const
{
    return (_pim_mrt.pim_node());
}

int
PimMfc::family() const
{
    return (_pim_mrt.family());
}

void
PimMfc::set_rp_addr(const IPvX& v)
{
    if (v == _rp_addr)
	return;			// Nothing changed
    
    uncond_set_rp_addr(v);
}

void
PimMfc::uncond_set_rp_addr(const IPvX& v)
{
    pim_node().rp_table().delete_pim_mfc(this);
    _rp_addr = v;
    pim_node().rp_table().add_pim_mfc(this);
}

void
PimMfc::recompute_rp_mfc()
{
    IPvX new_rp_addr(IPvX::ZERO(family()));
    
    PimRp *new_pim_rp = pim_node().rp_table().rp_find(group_addr());
    
    if (new_pim_rp != NULL)
	new_rp_addr = new_pim_rp->rp_addr();
    
    if (new_rp_addr == rp_addr())
	return;			// Nothing changed
    
    set_rp_addr(new_rp_addr);
    add_mfc_to_kernel();
    // XXX: we just recompute the state, hence no need to add
    // a dataflow monitor.
}

void
PimMfc::recompute_iif_olist_mfc()
{
    uint16_t new_iif_vif_index = Vif::VIF_INDEX_INVALID;
    Mifset new_olist;
    uint16_t old_iif_vif_index = iif_vif_index();
    Mifset old_olist = olist();
    uint32_t lookup_flags;
    PimMre *pim_mre, *pim_mre_sg;
    
    lookup_flags = PIM_MRE_RP | PIM_MRE_WC | PIM_MRE_SG | PIM_MRE_SG_RPT;
    pim_mre = pim_mrt().pim_mre_find(source_addr(), group_addr(),
				     lookup_flags, 0);
    if (pim_mre == NULL) {
	// No matching multicast routing entry. Remove the PimMfc entry.
	// TODO: XXX: PAVPAVPAV: do we really want to remove the entry?
	// E.g., just reset the olist, and leave the entry itself to timeout?
	delete this;	// XXX: this will remove it from the kernel
	
	return;
    }
    
    // Get the (S,G) PimMre entry (if exists)
    pim_mre_sg = NULL;
    do {
	if (pim_mre->is_sg()) {
	    pim_mre_sg = pim_mre;
	    break;
	}
	if (pim_mre->is_sg_rpt()) {
	    pim_mre_sg = pim_mre->sg_entry();
	    break;
	}
	break;
    } while (false);
    
    // Compute the iif and the olist
    if (pim_mre->is_sg()
	&& ((pim_mre->is_spt() && pim_mre->is_joined_state())
	    || (pim_mre->is_directly_connected_s()))) {
	new_iif_vif_index = pim_mre->rpf_interface_s();
    } else {
	new_iif_vif_index = pim_mre->rpf_interface_rp();
    }
    
    new_olist = pim_mre->inherited_olist_sg_forward();
    
    if (new_iif_vif_index == Vif::VIF_INDEX_INVALID) {
	// TODO: XXX: PAVPAVPAV: completely remove the olist check and comments
	// || new_olist.none()) {
	// No incoming interface or outgoing interfaces.
	// Remove the PimMfc entry.
	// TODO: XXX: PAVPAVPAV: do we really want to remove the entry?
	// E.g., just reset the olist, and leave the entry itself to timeout?
	
	// Cancel the (S,G) Keepalive Timer
	if ((pim_mre_sg != NULL) && pim_mre_sg->is_keepalive_timer_running()) {
	    pim_mre_sg->cancel_keepalive_timer();
	    pim_mre_sg->entry_try_remove();
	}
	
	delete this;	// XXX: this will remove it from the kernel
	
	return;
    }
    
    if ((new_iif_vif_index == old_iif_vif_index)
	&& (new_olist == old_olist)) {
	return;			// Nothing changed
    }
    
    if ((old_iif_vif_index != Vif::VIF_INDEX_INVALID)
	&& (old_olist.none())) {
	// XXX: probably an entry that was installed to stop NOCACHE upcalls,
	// or that was left around until the (S,G) NotJoined routing state
	// expires. Just delete the PimMfc entry, and then later when we are
	// forced to install a new PimMfc entry because of NOCACHE upcall,
	// we will set appropriately the SPT bit, etc.
	
	// Cancel the (S,G) Keepalive Timer
	if ((pim_mre_sg != NULL) && pim_mre_sg->is_keepalive_timer_running()) {
	    pim_mre_sg->cancel_keepalive_timer();
	    pim_mre_sg->entry_try_remove();
	}
	
	delete this;
	
	return;
    }
    
    set_iif_vif_index(new_iif_vif_index);
    set_olist(new_olist);
    add_mfc_to_kernel();
    // XXX: we just recompute the state, hence no need to add
    // a dataflow monitor.
}

int
PimMfc::add_mfc_to_kernel()
{
    do {
	string res;
	for (size_t i = 0; i < pim_node().maxvifs(); i++) {
	    if (olist().test(i))
		res += "O";
	    else
		res += ".";
	}
	XLOG_TRACE(pim_node().is_log_trace(),
		   "Add MFC entry: (%s,%s) iif = %d olist = %s",
		   cstring(source_addr()),
		   cstring(group_addr()),
		   iif_vif_index(),
		   res.c_str());
    } while (false);
    
    if (pim_node().add_mfc_to_kernel(*this) < 0)
	return (XORP_ERROR);
    
    return (XORP_OK);
}

int
PimMfc::delete_mfc_from_kernel()
{
    do {
	string res;
	for (size_t i = 0; i < pim_node().maxvifs(); i++) {
	    if (olist().test(i))
		res += "O";
	    else
		res += ".";
	}
	XLOG_TRACE(pim_node().is_log_trace(),
		   "Delete MFC entry: (%s,%s) iif = %d olist = %s",
		   cstring(source_addr()),
		   cstring(group_addr()),
		   iif_vif_index(),
		   res.c_str());
    } while (false);
    
    delete_all_dataflow_monitor();
    if (pim_node().delete_mfc_from_kernel(*this) < 0)
	return (XORP_ERROR);
    
    return (XORP_OK);
}

int
PimMfc::add_dataflow_monitor(uint32_t threshold_interval_sec,
			     uint32_t threshold_interval_usec,
			     uint32_t threshold_packets,
			     uint32_t threshold_bytes,
			     bool is_threshold_in_packets,
			     bool is_threshold_in_bytes,
			     bool is_geq_upcall,
			     bool is_leq_upcall)
{
    if (pim_node().add_dataflow_monitor(source_addr(),
					group_addr(),
					threshold_interval_sec,
					threshold_interval_usec,
					threshold_packets,
					threshold_bytes,
					is_threshold_in_packets,
					is_threshold_in_bytes,
					is_geq_upcall,
					is_leq_upcall) < 0) {
	return (XORP_ERROR);
    }
    
    set_has_dataflow_monitor(true);
    
    return (XORP_OK);
}

int
PimMfc::delete_dataflow_monitor(uint32_t threshold_interval_sec,
				uint32_t threshold_interval_usec,
				uint32_t threshold_packets,
				uint32_t threshold_bytes,
				bool is_threshold_in_packets,
				bool is_threshold_in_bytes,
				bool is_geq_upcall,
				bool is_leq_upcall)
{
    if (pim_node().delete_dataflow_monitor(source_addr(),
					   group_addr(),
					   threshold_interval_sec,
					   threshold_interval_usec,
					   threshold_packets,
					   threshold_bytes,
					   is_threshold_in_packets,
					   is_threshold_in_bytes,
					   is_geq_upcall,
					   is_leq_upcall) < 0) {
	return (XORP_ERROR);
    }
    
    set_has_dataflow_monitor(true);
    
    return (XORP_OK);
}

int
PimMfc::delete_all_dataflow_monitor()
{
    set_has_dataflow_monitor(false);
    
    if (pim_node().delete_all_dataflow_monitor(source_addr(),
					       group_addr()) < 0) {
	return (XORP_ERROR);
    }
    
    return (XORP_OK);
}

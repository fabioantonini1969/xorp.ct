// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2003 International Computer Science Institute
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

#ident "$XORP: xorp/rib/vifmanager.cc,v 1.14 2003/05/20 17:31:28 pavlin Exp $"

#include "rib_module.h"
#include "libxorp/xorp.h"
#include "libxorp/xlog.h"
#include "libxorp/debug.h"
#include "libxorp/ipvx.hh"
#include "libxorp/status_codes.h"

#include "libxipc/xrl_router.hh"

#include "rib_manager.hh"
#include "vifmanager.hh"

VifManager::VifManager(XrlRouter& xrl_router,
		       EventLoop& eventloop,
		       RibManager* rib_manager)
    : _xrl_router(xrl_router),
      _eventloop(eventloop),
      _rib_manager(rib_manager),
      _ifmgr_client(&xrl_router)
{
    _no_fea = false;
    _state = INITIALIZING;
    _interfaces_remaining = 0;
    _vifs_remaining = 0;
    _addrs_remaining = 0;

    _fea_target_name = "fea";		// XXX
    
    enable();	// XXX: by default the VifManager is always enabled
}

VifManager::~VifManager()
{
    stop();
    
    // Remove all Vif entries
    map <string, Vif*>::iterator iter;
    for (iter = _vifs_by_name.begin(); iter != _vifs_by_name.end(); ++iter) {
	delete iter->second;
    }
    for (iter = _saved_vifs_by_name.begin();
	 iter != _saved_vifs_by_name.end(); ++iter) {
	delete iter->second;
    }
}

/**
 * Start operation.
 * 
 * Start the process of registering with the FEA, etc.
 * 
 * @return XORP_OK on success, otherwise XORP_ERROR.
 */
int
VifManager::start()
{
    enable();	// XXX: by default the VifManager is always enabled
    
    if (ProtoState::start() < 0)
	return (XORP_ERROR);
    
    if (_no_fea) {
	_state = READY;
	return (XORP_OK);
    }
    
    register_if_spy();
    return (XORP_OK);
}

/**
 * Stop operation.
 * 
 * Gracefully stop operation.
 * 
 * @return XORP_OK on success, otherwise XORP_ERROR.
 */
int
VifManager::stop()
{
    if (! is_up())
        return (XORP_ERROR);
    
    clean_out_old_state();
    
    ProtoState::stop();
    
    return (XORP_OK);
}

void
VifManager::update_state()
{
    switch (_state) {
    case INITIALIZING:
	if ((_interfaces_remaining == 0)
	    && (_vifs_remaining == 0)
	    && (_addrs_remaining == 0)) {
	    _state = READY;
	}
	break;
    default:
	break;
    }
    
    // Time to set the vif state
    if (_state == READY) {
	if ((_interfaces_remaining == 0)
	    && (_vifs_remaining == 0)
	    && (_addrs_remaining == 0)) {
	    set_vif_state();
	}
    }
}

void
VifManager::set_vif_state()
{
    map<string, Vif *>::const_iterator iter, vif_iter;
    string err;
    
    //
    // Remove vifs that don't exist anymore
    //
    for (vif_iter = _saved_vifs_by_name.begin();
	 vif_iter != _saved_vifs_by_name.end();
	) {
	Vif* node_vif = vif_iter->second;
	++vif_iter;
	if (node_vif->is_pim_register())
	    continue;		// XXX: don't delete the PIM Register vif
	if (_vifs_by_name.find(node_vif->name()) == _vifs_by_name.end()) {
	    // Delete the interface
	    string vif_name = node_vif->name();
	    _rib_manager->delete_vif(vif_name, err);
	    _saved_vifs_by_name.erase(node_vif->name());
	    delete node_vif;
	    continue;
	}
    }
    
    //
    // Add new vifs, and update existing ones
    //
    for (iter = _vifs_by_name.begin(); iter != _vifs_by_name.end(); ++iter) {
	Vif* vif = iter->second;
	Vif* node_vif = NULL;
	
	vif_iter = _saved_vifs_by_name.find(vif->name());
	if (vif_iter != _saved_vifs_by_name.end())
	    node_vif = vif_iter->second;
	
	//
	// Add a new vif
	//
	if (node_vif == NULL) {
	    _rib_manager->new_vif(vif->name(), *vif, err);
	    node_vif = new Vif(*vif);
	    _saved_vifs_by_name.insert(pair<string, Vif*>(vif->name(),
							  node_vif));
	    continue;
	}
	
	//
	// Update the vif flags
	//
	// TODO: if necessary, we should update the vif flags inside the RIB
	node_vif->set_p2p(vif->is_p2p());
	node_vif->set_loopback(vif->is_loopback());
	node_vif->set_multicast_capable(vif->is_multicast_capable());
	node_vif->set_broadcast_capable(vif->is_broadcast_capable());
	node_vif->set_underlying_vif_up(vif->is_underlying_vif_up());
	
	//
	// Delete vif addresses that don't exist anymore
	//
	{
	    list<IPvX> delete_addresses_list;
	    list<VifAddr>::const_iterator vif_addr_iter;
	    for (vif_addr_iter = node_vif->addr_list().begin();
		 vif_addr_iter != node_vif->addr_list().end();
		 ++vif_addr_iter) {
		const VifAddr& vif_addr = *vif_addr_iter;
		if (vif->find_address(vif_addr.addr()) == NULL)
		    delete_addresses_list.push_back(vif_addr.addr());
	    }
	    // Delete the addresses
	    list<IPvX>::iterator ipvx_iter;
	    for (ipvx_iter = delete_addresses_list.begin();
		 ipvx_iter != delete_addresses_list.end();
		 ++ipvx_iter) {
		const IPvX& ipvx = *ipvx_iter;
		if (ipvx.is_ipv4()) {
		    _rib_manager->delete_vif_addr(node_vif->name(),
						  ipvx.get_ipv4(),
						  err);
		}
		if (ipvx.is_ipv6()) {
		    _rib_manager->delete_vif_addr(node_vif->name(),
						  ipvx.get_ipv6(),
						  err);
		}
		node_vif->delete_address(ipvx);
	    }
	}
	
	//
	// Add new vif addresses, and update existing ones
	//
	{
	    list<VifAddr>::const_iterator vif_addr_iter;
	    for (vif_addr_iter = vif->addr_list().begin();
		 vif_addr_iter != vif->addr_list().end();
		 ++vif_addr_iter) {
		const VifAddr& vif_addr = *vif_addr_iter;
		VifAddr* node_vif_addr = node_vif->find_address(vif_addr.addr());
		if (node_vif_addr == NULL) {
		    if (vif_addr.addr().is_ipv4()) {
			_rib_manager->add_vif_addr(node_vif->name(),
						   vif_addr.addr().get_ipv4(),
						   vif_addr.subnet_addr().get_ipv4net(),
						   err);
		    }
		    if (vif_addr.addr().is_ipv6()) {
			_rib_manager->add_vif_addr(node_vif->name(),
						   vif_addr.addr().get_ipv6(),
						   vif_addr.subnet_addr().get_ipv6net(),
						   err);
		    }
		    node_vif->add_address(vif_addr);
		    continue;
		}
		// Update the address
		// TODO: if necessary, we should update the address inside
		// the RIB
		*node_vif_addr = vif_addr;
	    }
	}
    }
}

void
VifManager::clean_out_old_state()
{
    if (_no_fea)
	return;
    
    //
    // We call unregister_client first, to cause the FEA to remove any
    // registrations left over from previous incarnations of the RIB.
    //
    XorpCallback1<void, const XrlError&>::RefPtr cb;
    cb = callback(this, &VifManager::xrl_result_unregister_client);
    _ifmgr_client.send_unregister_client(_fea_target_name.c_str(),
					 _xrl_router.name(), cb);
}

void
VifManager::xrl_result_unregister_client(const XrlError& e)
{
    UNUSED(e);
    
    //
    // XXX: We really don't care here if the request succeeded or failed.
    // It is normal to fail with COMMAND_FAILED if there was no state
    // left behind from a previous incarnation. Any other errors would
    // also show up in register_if_spy, so we'll let that deal with
    // them.
    //
    // register_if_spy();
}

void
VifManager::register_if_spy()
{
    XorpCallback1<void, const XrlError&>::RefPtr cb;
    cb = callback(this, &VifManager::xrl_result_register_client);
    _ifmgr_client.send_register_client(_fea_target_name.c_str(),
				       _xrl_router.name(), cb);
}

void
VifManager::xrl_result_register_client(const XrlError& e)
{
    if (_no_fea) {
	_state = READY;
	return;
    }
    
    if (e == XrlError::OKAY()) {
	//
	// The registration was successful. Now we need to query the
	// entries that are already there. First, find out the set of
	// all interfaces.
	//
	XorpCallback2<void, const XrlError&, const XrlAtomList*>::RefPtr cb;
	cb = callback(this, &VifManager::xrl_result_get_configured_interface_names);
	_ifmgr_client.send_get_configured_interface_names(_fea_target_name.c_str(),
							  cb);
	return;
    }
    
    //
    // If the resolve failed, it could be that we got going too quickly
    // for the FEA. Retry every two seconds.
    //
    if (e == XrlError::RESOLVE_FAILED()) {
	debug_msg("Register Interface Spy: RESOLVE_FAILED\n");
	OneoffTimerCallback cb;
	cb = callback(this, &VifManager::register_if_spy);
	_register_retry_timer = _eventloop.new_oneoff_after(TimeVal(2, 0), cb);
	return;
    }
    
    // Permanent error
    _state = FAILED;
    XLOG_ERROR("Register Interface Spy: Permanent Error");
}

void
VifManager::xrl_result_get_configured_interface_names(
    const XrlError& e,
    const XrlAtomList* alist)
{
    if (e == XrlError::OKAY()) {
	for (size_t i = 0; i < alist->size(); i++) {
	    //
	    // Spin through the list of interfaces, and fire off
	    // requests in parallel for all the Vifs on each interface.
	    //
	    XrlAtom atom = alist->get(i);
	    string ifname = atom.text();
	    debug_msg("got interface name: %s\n", ifname.c_str());
	    
	    XorpCallback2<void, const XrlError&, const XrlAtomList*>::RefPtr cb;
	    cb = callback(this, &VifManager::xrl_result_get_configured_vif_names, ifname);
	    _ifmgr_client.send_get_configured_vif_names(_fea_target_name.c_str(),
							ifname, cb);
	    _interfaces_remaining++;
	}
	return;
    }
    
    // Permanent error
    _state = FAILED;
    XLOG_ERROR("Get Interface Names: Permanent Error");
}

void
VifManager::xrl_result_get_configured_vif_names(
    const XrlError& e,
    const XrlAtomList* alist,
    string ifname)
{
    if (_interfaces_remaining == 0) {
	// Unexpected response
	XLOG_WARNING("Received unexpected XRL response for "
		     "get_configured_vif_names for interface %s",
		     ifname.c_str());
	return;
    }
    
    _interfaces_remaining--;
    
    if (e == XrlError::OKAY()) {
	// Spin through all the Vifs on this interface, and create them.
	for (size_t i = 0; i < alist->size(); i++) {
	    XrlAtom atom = alist->get(i);
	    string vifname = atom.text();
	    vif_created(ifname, vifname);
	}
	update_state();
	return;
    }
    
    if (e == XrlError::COMMAND_FAILED()) {
	// perhaps the interface went away
	update_state();
	return;
    }
    
    // Permanent error
    _state = FAILED;
    XLOG_ERROR("Get VIF Names: Permanent Error");
}

void
VifManager::xrl_result_get_configured_vif_flags(const XrlError& e,
						const bool* enabled,
						const bool* broadcast,
						const bool* loopback,
						const bool* point_to_point,
						const bool* multicast,
						string ifname,
						string vifname)
{
    if (_vifs_remaining == 0) {
	// Unexpected response
	XLOG_WARNING("Received unexpected XRL response for "
		     "get_configured_vif_flags for interface %s, vif %s",
		     ifname.c_str(), vifname.c_str());
	return;
    }
    
    _vifs_remaining--;
    
    if (e == XrlError::OKAY()) {
	if (_vifs_by_name.find(vifname) == _vifs_by_name.end()) {
	    // silently ignore - the vif could have been deleted while we
	    // were waiting for the answer.
	    update_state();
	    return;
	}
	Vif* vif = _vifs_by_name[vifname];
	debug_msg("setting flags for vif %s\n",
		  vifname.c_str());
	vif->set_underlying_vif_up(*enabled);
	vif->set_broadcast_capable(*broadcast);
	vif->set_loopback(*loopback);
	vif->set_p2p(*point_to_point);
	vif->set_multicast_capable(*multicast);
	
	update_state();
	return;
    }
    
    if (e == XrlError::COMMAND_FAILED()) {
	// perhaps the vif went away
	update_state();
	return;
    }
    
    // Permanent error
    _state = FAILED;
    XLOG_ERROR("Failed to get flags for vif %s: Permanent Error",
	       vifname.c_str());
}

void
VifManager::xrl_result_get_configured_vif_addresses4(
    const XrlError& e,
    const XrlAtomList* alist,
    string ifname,
    string vifname)
{
    if (_vifs_remaining == 0) {
	// Unexpected response
	XLOG_WARNING("Received unexpected XRL response for "
		     "get_configured_vif_addresses4 for interface %s, vif %s",
		     ifname.c_str(), vifname.c_str());
	return;
    }
    
    _vifs_remaining--;
    
    if (e == XrlError::OKAY()) {
	for (size_t i = 0; i < alist->size(); i++) {
	    XrlAtom atom = alist->get(i);
	    IPv4 addr = atom.ipv4();
	    vifaddr4_created(ifname, vifname, addr);
	}
	update_state();
	return;
    }
    
    if (e == XrlError::COMMAND_FAILED()) {
	// perhaps the vif went away
	update_state();
	return;
    }
    
    // Permanent error
    _state = FAILED;
    XLOG_ERROR("Get VIF addresses: Permanent Error");
}

void
VifManager::xrl_result_get_configured_vif_addresses6(
    const XrlError& e,
    const XrlAtomList* alist,
    string ifname,
    string vifname)
{
    if (_vifs_remaining == 0) {
	// Unexpected response
	XLOG_WARNING("Received unexpected XRL response for "
		     "get_configured_vif_addresses6 for interface %s, vif %s",
		     ifname.c_str(), vifname.c_str());
	return;
    }
    
    _vifs_remaining--;
    
    if (e == XrlError::OKAY()) {
	for (size_t i = 0; i < alist->size(); i++) {
	    XrlAtom atom = alist->get(i);
	    IPv6 addr = atom.ipv6();
	    vifaddr6_created(ifname, vifname, addr);
	}
	update_state();
	return;
    }
    
    if (e == XrlError::COMMAND_FAILED()) {
	// perhaps the vif went away
	update_state();
	return;
    }
    
    // Permanent error
    _state = FAILED;
    XLOG_ERROR("Get VIF addresses: Permanent Error");
}

void
VifManager::interface_update(const string& ifname,
			     const uint32_t& event)
{
    switch (event) {
    case IF_EVENT_CREATED:
	// doesn't directly affect vifs - we'll get a vif_update for
	// any new vifs
	break;
    case IF_EVENT_DELETED:
	interface_deleted(ifname);
	break;
    case IF_EVENT_CHANGED:
	// doesn't directly affect vifs
	break;
    default:
	XLOG_WARNING("interface_update invalid event: %u", event);
	break;
    }
}

void
VifManager::vif_update(const string& ifname, const string& vifname,
		       const uint32_t& event)
{
    switch (event) {
    case IF_EVENT_CREATED:
	vif_created(ifname, vifname);
	break;
    case IF_EVENT_DELETED:
	vif_deleted(ifname, vifname);
	break;
    case IF_EVENT_CHANGED:
	// doesn't directly affect us
	break;
    default:
	XLOG_WARNING("vif_update invalid event: %u", event);
	break;
    }
}

void
VifManager::vifaddr4_update(const string& ifname,
			    const string& vifname,
			    const IPv4& addr,
			    const uint32_t& event)
{
    switch (event) {
    case IF_EVENT_CREATED:
	vifaddr4_created(ifname, vifname, addr);
	break;
    case IF_EVENT_DELETED:
	vifaddr4_deleted(ifname, vifname, addr);
	break;
    case IF_EVENT_CHANGED:
	// XXX: force query address-related info
	vifaddr4_created(ifname, vifname, addr);
	break;
    default:
	XLOG_WARNING("vifaddr4_update invalid event: %u", event);
	break;
    }
}

void
VifManager::vifaddr6_update(const string& ifname,
			    const string& vifname,
			    const IPv6& addr,
			    const uint32_t& event)
{
    switch (event) {
    case IF_EVENT_CREATED:
	vifaddr6_created(ifname, vifname, addr);
	break;
    case IF_EVENT_DELETED:
	vifaddr6_deleted(ifname, vifname, addr);
	break;
    case IF_EVENT_CHANGED:
	// XXX: force query address-related info
	vifaddr6_created(ifname, vifname, addr);
	break;
    default:
	XLOG_WARNING("vifaddr6_update invalid event: %u", event);
	break;
    }
}

void
VifManager::interface_deleted(const string& ifname)
{
    // Reomve all vifs for the same interface name
    multimap<string, Vif*>::iterator iter;
    iter = _vifs_by_interface.find(ifname);
    for (; iter != _vifs_by_interface.end(); ++iter) {
	if (iter->first != ifname)
	    break;
	vif_deleted(ifname, iter->second->name());
    }
}

void
VifManager::vif_deleted(const string& ifname, const string& vifname)
{
    debug_msg("vif_deleted %s %s\n", ifname.c_str(), vifname.c_str());
    
    map<string, Vif*>::iterator vi = _vifs_by_name.find(vifname);
    if (vi == _vifs_by_name.end()) {
	XLOG_ERROR("vif_deleted: vif %s not found", vifname.c_str());
	return;
    }
    
    //
    // Remove the vif from the local maps
    //
    Vif* vif = vi->second;
    _vifs_by_name.erase(vi);
    multimap<string, Vif*>::iterator ii = _vifs_by_interface.find(ifname);
    XLOG_ASSERT(ii != _vifs_by_interface.end());
    while (ii != _vifs_by_interface.end() && ii->first == ifname) {
	if (ii->second == vif) {
	    _vifs_by_interface.erase(ii);
	    break;
	}
	++ii;
	XLOG_ASSERT(ii != _vifs_by_interface.end());
    }
    delete vif;
    
    update_state();
}

void
VifManager::vif_created(const string& ifname, const string& vifname)
{
    debug_msg("vif_created: %s\n", vifname.c_str());
    
    if (_vifs_by_name.find(vifname) != _vifs_by_name.end()) {
	XLOG_ERROR("vif_created: vif %s already exists", vifname.c_str());
	return;
    }
    
    //
    // Create a new vif
    //
    Vif *vif = new Vif(vifname, ifname);
    _vifs_by_name[vifname] = vif;
    _vifs_by_interface.insert(pair<string, Vif*>(ifname, vif));
    
    //
    // Fire off requests in parallel to get the flags and all the
    // addresses on each Vif.
    //
    {
	// Get the vif flags
	XorpCallback6<void, const XrlError&, const bool*, const bool*,
	    const bool*, const bool*, const bool*>::RefPtr cb;
	cb = callback(this, &VifManager::xrl_result_get_configured_vif_flags,
		      ifname, vifname);
	_ifmgr_client.send_get_configured_vif_flags(_fea_target_name.c_str(),
						    ifname, vifname, cb);
	_vifs_remaining++;
    }
    
    XorpCallback2<void, const XrlError&, const XrlAtomList*>::RefPtr cb;
    {
	// Get IPv4 addresses
	cb = callback(this, &VifManager::xrl_result_get_configured_vif_addresses4,
		      ifname, vifname);
	_ifmgr_client.send_get_configured_vif_addresses4(_fea_target_name.c_str(),
							 ifname,
							 vifname,
							 cb);
	_vifs_remaining++;
    }
    {
	// Get IPv6 addresses
	cb = callback(this, &VifManager::xrl_result_get_configured_vif_addresses6,
		      ifname, vifname);
	_ifmgr_client.send_get_configured_vif_addresses6(_fea_target_name.c_str(),
							 ifname,
							 vifname,
							 cb);
	_vifs_remaining++;
    }
    
    update_state();
}

void
VifManager::vifaddr4_created(const string& ifname,
			     const string& vifname,
			     const IPv4& addr)
{
    if (_vifs_by_name.find(vifname) == _vifs_by_name.end()) {
	XLOG_ERROR("vifaddr4_created on unknown vif: %s", vifname.c_str());
	return;
    }
    
    {
	// Get the address flags
	XorpCallback6<void, const XrlError&, const bool*, const bool*,
	    const bool*, const bool*, const bool*>::RefPtr cb;
	cb = callback(this, &VifManager::xrl_result_get_configured_address_flags4,
		      ifname, vifname, addr);
	_ifmgr_client.send_get_configured_address_flags4(_fea_target_name.c_str(),
							 ifname, vifname, addr, cb);
	_addrs_remaining++;
    }
    
    {
	// Get the prefix length
	XorpCallback2<void, const XrlError&, const uint32_t*>::RefPtr cb;
	cb = callback(this, &VifManager::xrl_result_get_configured_prefix4,
		      ifname, vifname, addr);
	_ifmgr_client.send_get_configured_prefix4(_fea_target_name.c_str(), ifname,
						  vifname, addr, cb);
	_addrs_remaining++;
    }
}

void
VifManager::vifaddr6_created(const string& ifname,
			     const string& vifname,
			     const IPv6& addr)
{
    if (_vifs_by_name.find(vifname) == _vifs_by_name.end()) {
	XLOG_ERROR("vifaddr6_created on unknown vif: %s", vifname.c_str());
	return;
    }
    
    {
	// Get the address flags
	XorpCallback5<void, const XrlError&, const bool*,
	    const bool*, const bool*, const bool*>::RefPtr cb;
	cb = callback(this, &VifManager::xrl_result_get_configured_address_flags6,
		      ifname, vifname, addr);
	_ifmgr_client.send_get_configured_address_flags6(_fea_target_name.c_str(),
							 ifname, vifname, addr, cb);
	_addrs_remaining++;
    }
    
    {
	// Get the prefix length
	XorpCallback2<void, const XrlError&, const uint32_t*>::RefPtr cb;
	cb = callback(this, &VifManager::xrl_result_get_configured_prefix6,
		      ifname, vifname, addr);
	_ifmgr_client.send_get_configured_prefix6(_fea_target_name.c_str(), ifname,
						  vifname, addr, cb);
	_addrs_remaining++;
    }
}

void
VifManager::xrl_result_get_configured_address_flags4(const XrlError& e,
						     const bool* enabled,
						     const bool* broadcast,
						     const bool* loopback,
						     const bool* point_to_point,
						     const bool* multicast,
						     string ifname,
						     string vifname,
						     IPv4 addr)
{
    if (_addrs_remaining == 0) {
	// Unexpected response
	XLOG_WARNING("Received unexpected XRL response for "
		     "get_configured_address_flags4 for interface %s, vif %s",
		     ifname.c_str(), vifname.c_str());
	return;
    }
    
    _addrs_remaining--;
    
    if (e == XrlError::OKAY()) {
	if (_vifs_by_name.find(vifname) == _vifs_by_name.end()) {
	    // silently ignore - the vif could have been deleted while we
	    // were waiting for the answer.
	    update_state();
	    return;
	}
	Vif* vif = _vifs_by_name[vifname];
	debug_msg("setting flags for address %s to vif %s\n",
		  addr.str().c_str(), vifname.c_str());
	vif->set_underlying_vif_up(*enabled);
	vif->set_broadcast_capable(*broadcast);
	vif->set_loopback(*loopback);
	vif->set_p2p(*point_to_point);
	vif->set_multicast_capable(*multicast);

	if (*broadcast) {
	    // Get the broadcast address
	    XorpCallback2<void, const XrlError&, const IPv4*>::RefPtr cb;
	    cb = callback(this, &VifManager::xrl_result_get_configured_broadcast4,
			  ifname, vifname, addr);
	    _ifmgr_client.send_get_configured_broadcast4(_fea_target_name.c_str(),
							 ifname, vifname, addr, cb);
	    _addrs_remaining++;
	}
	
	if (*point_to_point) {
	    // Get the endpoint address
	    XorpCallback2<void, const XrlError&, const IPv4*>::RefPtr cb;
	    cb = callback(this, &VifManager::xrl_result_get_configured_endpoint4,
			  ifname, vifname, addr);
	    _ifmgr_client.send_get_configured_endpoint4(_fea_target_name.c_str(),
							ifname, vifname, addr, cb);
	    _addrs_remaining++;
	}
	
	update_state();
	return;
    }
    
    if (e == XrlError::COMMAND_FAILED()) {
	// perhaps the vif went away
	update_state();
	return;
    }
    
    // Permanent error
    _state = FAILED;
    XLOG_ERROR("Failed to get flags for address %s: Permanent Error",
	       addr.str().c_str());
}

void
VifManager::xrl_result_get_configured_address_flags6(const XrlError& e,
						     const bool* enabled,
						     const bool* loopback,
						     const bool* point_to_point,
						     const bool* multicast,
						     string ifname,
						     string vifname,
						     IPv6 addr)
{
    if (_addrs_remaining == 0) {
	// Unexpected response
	XLOG_WARNING("Received unexpected XRL response for "
		     "get_configured_address_flags6 for interface %s, vif %s",
		     ifname.c_str(), vifname.c_str());
	return;
    }
    
    _addrs_remaining--;
    
    if (e == XrlError::OKAY()) {
	if (_vifs_by_name.find(vifname) == _vifs_by_name.end()) {
	    // silently ignore - the vif could have been deleted while we
	    // were waiting for the answer.
	    update_state();
	    return;
	}
	Vif* vif = _vifs_by_name[vifname];
	debug_msg("setting flags for address %s to vif %s\n",
		  addr.str().c_str(), vifname.c_str());
	vif->set_underlying_vif_up(*enabled);
	vif->set_loopback(*loopback);
	vif->set_p2p(*point_to_point);
	vif->set_multicast_capable(*multicast);
	
	{
	    // Get the broadcast address
	    // XXX: IPv6 doesn't have broadcast addresses, hence we don't do it
	}
	
	if (*point_to_point) {
	    // Get the endpoint address
	    XorpCallback2<void, const XrlError&, const IPv6*>::RefPtr cb;
	    cb = callback(this, &VifManager::xrl_result_get_configured_endpoint6,
			  ifname, vifname, addr);
	    _ifmgr_client.send_get_configured_endpoint6(_fea_target_name.c_str(),
							ifname, vifname, addr, cb);
	    _addrs_remaining++;
	}
	
	update_state();
	return;
    }
    
    if (e == XrlError::COMMAND_FAILED()) {
	// perhaps the vif went away
	update_state();
	return;
    }
    
    // Permanent error
    _state = FAILED;
    XLOG_ERROR("Failed to get flags for address %s: Permanent Error",
	       addr.str().c_str());
}

void
VifManager::xrl_result_get_configured_prefix4(const XrlError& e,
					      const uint32_t* prefix_len,
					      string ifname, string vifname,
					      IPv4 addr)
{
    if (_addrs_remaining == 0) {
	// Unexpected response
	XLOG_WARNING("Received unexpected XRL response for "
		     "get_configured_prefix4 for interface %s, vif %s",
		     ifname.c_str(), vifname.c_str());
	return;
    }
    
    _addrs_remaining--;
    
    if (e == XrlError::OKAY()) {
	if (_vifs_by_name.find(vifname) == _vifs_by_name.end()) {
	    // silently ignore - the vif could have been deleted while we
	    // were waiting for the answer.
	    update_state();
	    return;
	}
	Vif* vif = _vifs_by_name[vifname];
	debug_msg("adding address %s prefix_len %d to vif %s\n",
		  addr.str().c_str(), *prefix_len, vifname.c_str());
	VifAddr* vif_addr = vif->find_address(IPvX(addr));
	if (vif_addr == NULL) {
	    vif->add_address(IPvX(addr));
	    vif_addr = vif->find_address(IPvX(addr));
	}
	XLOG_ASSERT(vif_addr != NULL);
	vif_addr->set_subnet_addr(IPvXNet(IPvX(addr), *prefix_len));
	update_state();
	return;
    }
    
    if (e == XrlError::COMMAND_FAILED()) {
	// perhaps the vif went away
	update_state();
	return;
    }
    
    // Permanent error
    _state = FAILED;
    XLOG_ERROR("Failed to get prefix_len for address %s: Permanent Error",
	       addr.str().c_str());
}

void
VifManager::xrl_result_get_configured_prefix6(const XrlError& e,
					      const uint32_t* prefix_len,
					      string ifname, string vifname,
					      IPv6 addr)
{
    if (_addrs_remaining == 0) {
	// Unexpected response
	XLOG_WARNING("Received unexpected XRL response for "
		     "get_configured_prefix6 for interface %s, vif %s",
		     ifname.c_str(), vifname.c_str());
	return;
    }
    
    _addrs_remaining--;
    
    if (e == XrlError::OKAY()) {
	if (_vifs_by_name.find(vifname) == _vifs_by_name.end()) {
	    // silently ignore - the vif could have been deleted while we
	    // were waiting for the answer.
	    update_state();
	    return;
	}
	Vif *vif = _vifs_by_name[vifname];
	debug_msg("adding address %s prefix_len %d to vif %s\n",
		  addr.str().c_str(), *prefix_len, vifname.c_str());
	VifAddr* vif_addr = vif->find_address(IPvX(addr));
	if (vif_addr == NULL) {
	    vif->add_address(IPvX(addr));
	    vif_addr = vif->find_address(IPvX(addr));
	}
	XLOG_ASSERT(vif_addr != NULL);
	vif_addr->set_subnet_addr(IPvXNet(IPvX(addr), *prefix_len));
	update_state();
	return;
    }
    
    if (e == XrlError::COMMAND_FAILED()) {
	// perhaps the vif went away
	update_state();
	return;
    }
    
    // Permanent error
    _state = FAILED;
    XLOG_ERROR("Failed to get prefix_len for address %s",
	       addr.str().c_str());
}

void
VifManager::xrl_result_get_configured_broadcast4(const XrlError& e,
						 const IPv4* broadcast,
						 string ifname, string vifname,
						 IPv4 addr)
{
    if (_addrs_remaining == 0) {
	// Unexpected response
	XLOG_WARNING("Received unexpected XRL response for "
		     "get_configured_broadcast4 for interface %s, vif %s",
		     ifname.c_str(), vifname.c_str());
	return;
    }
    
    _addrs_remaining--;
    
    if (e == XrlError::OKAY()) {
	if (_vifs_by_name.find(vifname) == _vifs_by_name.end()) {
	    // silently ignore - the vif could have been deleted while we
	    // were waiting for the answer.
	    update_state();
	    return;
	}
	Vif* vif = _vifs_by_name[vifname];
	debug_msg("adding address %s broadcast %s to vif %s\n",
		  addr.str().c_str(), broadcast->str().c_str(), vifname.c_str());
	VifAddr* vif_addr = vif->find_address(IPvX(addr));
	if (vif_addr == NULL) {
	    vif->add_address(IPvX(addr));
	    vif_addr = vif->find_address(IPvX(addr));
	}
	XLOG_ASSERT(vif_addr != NULL);
	vif_addr->set_broadcast_addr(IPvX(*broadcast));
	update_state();
	return;
    }
    
    if (e == XrlError::COMMAND_FAILED()) {
	// perhaps the vif went away
	update_state();
	return;
    }
    
    // Permanent error
    _state = FAILED;
    XLOG_ERROR("Failed to get broadcast addr for address %s: Permanent Error",
	       addr.str().c_str());
}

void
VifManager::xrl_result_get_configured_endpoint4(const XrlError& e,
						const IPv4* endpoint,
						string ifname, string vifname,
						IPv4 addr)
{
    if (_addrs_remaining == 0) {
	// Unexpected response
	XLOG_WARNING("Received unexpected XRL response for "
		     "get_configured_endpoint4 for interface %s, vif %s",
		     ifname.c_str(), vifname.c_str());
	return;
    }
    
    _addrs_remaining--;
    
    if (e == XrlError::OKAY()) {
	if (_vifs_by_name.find(vifname) == _vifs_by_name.end()) {
	    // silently ignore - the vif could have been deleted while we
	    // were waiting for the answer.
	    update_state();
	    return;
	}
	Vif* vif = _vifs_by_name[vifname];
	debug_msg("adding address %s endpoint %s to vif %s\n",
		  addr.str().c_str(), endpoint->str().c_str(), vifname.c_str());
	VifAddr* vif_addr = vif->find_address(IPvX(addr));
	if (vif_addr == NULL) {
	    vif->add_address(IPvX(addr));
	    vif_addr = vif->find_address(IPvX(addr));
	}
	XLOG_ASSERT(vif_addr != NULL);
	vif_addr->set_peer_addr(IPvX(*endpoint));
	update_state();
	return;
    }
    
    if (e == XrlError::COMMAND_FAILED()) {
	// perhaps the vif went away
	update_state();
	return;
    }
    
    // Permanent error
    _state = FAILED;
    XLOG_ERROR("Failed to get endpoint addr for address %s: Permanent Error",
	       addr.str().c_str());
}

void
VifManager::xrl_result_get_configured_endpoint6(const XrlError& e,
						const IPv6* endpoint,
						string ifname, string vifname,
						IPv6 addr)
{
    if (_addrs_remaining == 0) {
	// Unexpected response
	XLOG_WARNING("Received unexpected XRL response for "
		     "get_configured_endpoint6 for interface %s, vif %s",
		     ifname.c_str(), vifname.c_str());
	return;
    }
    
    _addrs_remaining--;
    
    if (e == XrlError::OKAY()) {
	if (_vifs_by_name.find(vifname) == _vifs_by_name.end()) {
	    // silently ignore - the vif could have been deleted while we
	    // were waiting for the answer.
	    update_state();
	    return;
	}
	Vif* vif = _vifs_by_name[vifname];
	debug_msg("adding address %s endpoint %s to vif %s\n",
		  addr.str().c_str(), endpoint->str().c_str(), vifname.c_str());
	VifAddr* vif_addr = vif->find_address(IPvX(addr));
	if (vif_addr == NULL) {
	    vif->add_address(IPvX(addr));
	    vif_addr = vif->find_address(IPvX(addr));
	}
	XLOG_ASSERT(vif_addr != NULL);
	vif_addr->set_peer_addr(IPvX(*endpoint));
	update_state();
	return;
    }
    
    if (e == XrlError::COMMAND_FAILED()) {
	// perhaps the vif went away
	update_state();
	return;
    }
    
    // Permanent error
    _state = FAILED;
    XLOG_ERROR("Failed to get endpoint addr for address %s: Permanent Error",
	       addr.str().c_str());
}

void
VifManager::vifaddr4_deleted(const string& ifname,
			     const string& vifname,
			     const IPv4& addr)
{
    UNUSED(ifname);
    if (_vifs_by_name.find(vifname) == _vifs_by_name.end()) {
	XLOG_ERROR("vifaddr4_deleted on unknown vif: %s", vifname.c_str());
	return;
    }
    Vif *vif = _vifs_by_name[vifname];
    vif->delete_address(addr);
    
    update_state();
}

void
VifManager::vifaddr6_deleted(const string& ifname,
			     const string& vifname,
			     const IPv6& addr)
{
    UNUSED(ifname);
    if (_vifs_by_name.find(vifname) == _vifs_by_name.end()) {
	XLOG_ERROR("vifaddr6_deleted on unknown vif: %s", vifname.c_str());
	return;
    }
    Vif *vif = _vifs_by_name[vifname];
    vif->delete_address(addr);
    
    update_state();
}

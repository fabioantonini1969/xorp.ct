/*
 * Copyright (c) 2001-2009 XORP, Inc.
 * See the XORP LICENSE.lgpl file for licensing, conditions, and warranties
 * on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'tgt-gen'.
 */

#ident "$XORP: xorp/xrl/targets/finder_base.cc,v 1.32 2008/10/02 21:58:46 bms Exp $"


#include "finder_base.hh"


XrlFinderTargetBase::XrlFinderTargetBase(XrlCmdMap* cmds)
    : _cmds(cmds)
{
    if (_cmds)
	add_handlers();
}

XrlFinderTargetBase::~XrlFinderTargetBase()
{
    if (_cmds)
	remove_handlers();
}

bool
XrlFinderTargetBase::set_command_map(XrlCmdMap* cmds)
{
    if (_cmds == 0 && cmds) {
        _cmds = cmds;
        add_handlers();
        return true;
    }
    if (_cmds && cmds == 0) {
	remove_handlers();
        _cmds = cmds;
        return true;
    }
    return false;
}

const XrlCmdError
XrlFinderTargetBase::handle_common_0_1_get_target_name(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(0), XORP_UINT_CAST(xa_inputs.size()), "common/0.1/get_target_name");
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    string name;
    try {
	XrlCmdError e = common_0_1_get_target_name(
	    name);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "common/0.1/get_target_name", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("name", name);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_common_0_1_get_version(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(0), XORP_UINT_CAST(xa_inputs.size()), "common/0.1/get_version");
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    string version;
    try {
	XrlCmdError e = common_0_1_get_version(
	    version);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "common/0.1/get_version", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("version", version);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_common_0_1_get_status(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(0), XORP_UINT_CAST(xa_inputs.size()), "common/0.1/get_status");
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    uint32_t status;
    string reason;
    try {
	XrlCmdError e = common_0_1_get_status(
	    status,
	    reason);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "common/0.1/get_status", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("status", status);
	pxa_outputs->add("reason", reason);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_common_0_1_shutdown(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(0), XORP_UINT_CAST(xa_inputs.size()), "common/0.1/shutdown");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = common_0_1_shutdown();
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "common/0.1/shutdown", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_register_finder_client(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 4) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(4), XORP_UINT_CAST(xa_inputs.size()), "finder/0.2/register_finder_client");
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    string out_cookie;
    try {
	XrlCmdError e = finder_0_2_register_finder_client(
	    xa_inputs.get(0, "instance_name").text(),
	    xa_inputs.get(1, "class_name").text(),
	    xa_inputs.get(2, "singleton").boolean(),
	    xa_inputs.get(3, "in_cookie").text(),
	    out_cookie);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder/0.2/register_finder_client", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("out_cookie", out_cookie);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_unregister_finder_client(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 1) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(1), XORP_UINT_CAST(xa_inputs.size()), "finder/0.2/unregister_finder_client");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = finder_0_2_unregister_finder_client(
	    xa_inputs.get(0, "instance_name").text());
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder/0.2/unregister_finder_client", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_set_finder_client_enabled(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(2), XORP_UINT_CAST(xa_inputs.size()), "finder/0.2/set_finder_client_enabled");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = finder_0_2_set_finder_client_enabled(
	    xa_inputs.get(0, "instance_name").text(),
	    xa_inputs.get(1, "enabled").boolean());
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder/0.2/set_finder_client_enabled", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_finder_client_enabled(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 1) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(1), XORP_UINT_CAST(xa_inputs.size()), "finder/0.2/finder_client_enabled");
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    bool enabled;
    try {
	XrlCmdError e = finder_0_2_finder_client_enabled(
	    xa_inputs.get(0, "instance_name").text(),
	    enabled);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder/0.2/finder_client_enabled", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("enabled", enabled);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_add_xrl(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 3) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(3), XORP_UINT_CAST(xa_inputs.size()), "finder/0.2/add_xrl");
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    string resolved_xrl_method_name;
    try {
	XrlCmdError e = finder_0_2_add_xrl(
	    xa_inputs.get(0, "xrl").text(),
	    xa_inputs.get(1, "protocol_name").text(),
	    xa_inputs.get(2, "protocol_args").text(),
	    resolved_xrl_method_name);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder/0.2/add_xrl", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("resolved_xrl_method_name", resolved_xrl_method_name);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_remove_xrl(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 1) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(1), XORP_UINT_CAST(xa_inputs.size()), "finder/0.2/remove_xrl");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = finder_0_2_remove_xrl(
	    xa_inputs.get(0, "xrl").text());
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder/0.2/remove_xrl", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_resolve_xrl(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 1) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(1), XORP_UINT_CAST(xa_inputs.size()), "finder/0.2/resolve_xrl");
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    XrlAtomList resolutions;
    try {
	XrlCmdError e = finder_0_2_resolve_xrl(
	    xa_inputs.get(0, "xrl").text(),
	    resolutions);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder/0.2/resolve_xrl", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("resolutions", resolutions);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_get_xrl_targets(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(0), XORP_UINT_CAST(xa_inputs.size()), "finder/0.2/get_xrl_targets");
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    XrlAtomList target_names;
    try {
	XrlCmdError e = finder_0_2_get_xrl_targets(
	    target_names);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder/0.2/get_xrl_targets", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("target_names", target_names);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_get_xrls_registered_by(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 1) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(1), XORP_UINT_CAST(xa_inputs.size()), "finder/0.2/get_xrls_registered_by");
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    XrlAtomList xrls;
    try {
	XrlCmdError e = finder_0_2_get_xrls_registered_by(
	    xa_inputs.get(0, "target_name").text(),
	    xrls);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder/0.2/get_xrls_registered_by", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("xrls", xrls);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_get_ipv4_permitted_hosts(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(0), XORP_UINT_CAST(xa_inputs.size()), "finder/0.2/get_ipv4_permitted_hosts");
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    XrlAtomList ipv4s;
    try {
	XrlCmdError e = finder_0_2_get_ipv4_permitted_hosts(
	    ipv4s);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder/0.2/get_ipv4_permitted_hosts", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("ipv4s", ipv4s);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_get_ipv4_permitted_nets(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(0), XORP_UINT_CAST(xa_inputs.size()), "finder/0.2/get_ipv4_permitted_nets");
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    XrlAtomList ipv4nets;
    try {
	XrlCmdError e = finder_0_2_get_ipv4_permitted_nets(
	    ipv4nets);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder/0.2/get_ipv4_permitted_nets", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("ipv4nets", ipv4nets);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_get_ipv6_permitted_hosts(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(0), XORP_UINT_CAST(xa_inputs.size()), "finder/0.2/get_ipv6_permitted_hosts");
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    XrlAtomList ipv6s;
    try {
	XrlCmdError e = finder_0_2_get_ipv6_permitted_hosts(
	    ipv6s);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder/0.2/get_ipv6_permitted_hosts", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("ipv6s", ipv6s);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_get_ipv6_permitted_nets(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(0), XORP_UINT_CAST(xa_inputs.size()), "finder/0.2/get_ipv6_permitted_nets");
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    XrlAtomList ipv6nets;
    try {
	XrlCmdError e = finder_0_2_get_ipv6_permitted_nets(
	    ipv6nets);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder/0.2/get_ipv6_permitted_nets", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("ipv6nets", ipv6nets);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_event_notifier_0_1_register_class_event_interest(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(2), XORP_UINT_CAST(xa_inputs.size()), "finder_event_notifier/0.1/register_class_event_interest");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = finder_event_notifier_0_1_register_class_event_interest(
	    xa_inputs.get(0, "requester_instance").text(),
	    xa_inputs.get(1, "class_name").text());
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder_event_notifier/0.1/register_class_event_interest", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_event_notifier_0_1_deregister_class_event_interest(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(2), XORP_UINT_CAST(xa_inputs.size()), "finder_event_notifier/0.1/deregister_class_event_interest");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = finder_event_notifier_0_1_deregister_class_event_interest(
	    xa_inputs.get(0, "requester_instance").text(),
	    xa_inputs.get(1, "class_name").text());
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder_event_notifier/0.1/deregister_class_event_interest", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_event_notifier_0_1_register_instance_event_interest(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(2), XORP_UINT_CAST(xa_inputs.size()), "finder_event_notifier/0.1/register_instance_event_interest");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = finder_event_notifier_0_1_register_instance_event_interest(
	    xa_inputs.get(0, "requester_instance").text(),
	    xa_inputs.get(1, "instance_name").text());
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder_event_notifier/0.1/register_instance_event_interest", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_event_notifier_0_1_deregister_instance_event_interest(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != %u) handling %s",
            XORP_UINT_CAST(2), XORP_UINT_CAST(xa_inputs.size()), "finder_event_notifier/0.1/deregister_instance_event_interest");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = finder_event_notifier_0_1_deregister_instance_event_interest(
	    xa_inputs.get(0, "requester_instance").text(),
	    xa_inputs.get(1, "instance_name").text());
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for %s failed: %s",
            		 "finder_event_notifier/0.1/deregister_instance_event_interest", e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::BadArgs& e) {
	XLOG_ERROR("Error decoding the arguments: %s", e.str().c_str());
	return XrlCmdError::BAD_ARGS(e.str());
    }
    return XrlCmdError::OKAY();
}

void
XrlFinderTargetBase::add_handlers()
{
	if (_cmds->add_handler("common/0.1/get_target_name",
	    callback(this, &XrlFinderTargetBase::handle_common_0_1_get_target_name)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "common/0.1/get_target_name");
	}
	if (_cmds->add_handler("common/0.1/get_version",
	    callback(this, &XrlFinderTargetBase::handle_common_0_1_get_version)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "common/0.1/get_version");
	}
	if (_cmds->add_handler("common/0.1/get_status",
	    callback(this, &XrlFinderTargetBase::handle_common_0_1_get_status)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "common/0.1/get_status");
	}
	if (_cmds->add_handler("common/0.1/shutdown",
	    callback(this, &XrlFinderTargetBase::handle_common_0_1_shutdown)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "common/0.1/shutdown");
	}
	if (_cmds->add_handler("finder/0.2/register_finder_client",
	    callback(this, &XrlFinderTargetBase::handle_finder_0_2_register_finder_client)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder/0.2/register_finder_client");
	}
	if (_cmds->add_handler("finder/0.2/unregister_finder_client",
	    callback(this, &XrlFinderTargetBase::handle_finder_0_2_unregister_finder_client)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder/0.2/unregister_finder_client");
	}
	if (_cmds->add_handler("finder/0.2/set_finder_client_enabled",
	    callback(this, &XrlFinderTargetBase::handle_finder_0_2_set_finder_client_enabled)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder/0.2/set_finder_client_enabled");
	}
	if (_cmds->add_handler("finder/0.2/finder_client_enabled",
	    callback(this, &XrlFinderTargetBase::handle_finder_0_2_finder_client_enabled)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder/0.2/finder_client_enabled");
	}
	if (_cmds->add_handler("finder/0.2/add_xrl",
	    callback(this, &XrlFinderTargetBase::handle_finder_0_2_add_xrl)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder/0.2/add_xrl");
	}
	if (_cmds->add_handler("finder/0.2/remove_xrl",
	    callback(this, &XrlFinderTargetBase::handle_finder_0_2_remove_xrl)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder/0.2/remove_xrl");
	}
	if (_cmds->add_handler("finder/0.2/resolve_xrl",
	    callback(this, &XrlFinderTargetBase::handle_finder_0_2_resolve_xrl)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder/0.2/resolve_xrl");
	}
	if (_cmds->add_handler("finder/0.2/get_xrl_targets",
	    callback(this, &XrlFinderTargetBase::handle_finder_0_2_get_xrl_targets)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder/0.2/get_xrl_targets");
	}
	if (_cmds->add_handler("finder/0.2/get_xrls_registered_by",
	    callback(this, &XrlFinderTargetBase::handle_finder_0_2_get_xrls_registered_by)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder/0.2/get_xrls_registered_by");
	}
	if (_cmds->add_handler("finder/0.2/get_ipv4_permitted_hosts",
	    callback(this, &XrlFinderTargetBase::handle_finder_0_2_get_ipv4_permitted_hosts)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder/0.2/get_ipv4_permitted_hosts");
	}
	if (_cmds->add_handler("finder/0.2/get_ipv4_permitted_nets",
	    callback(this, &XrlFinderTargetBase::handle_finder_0_2_get_ipv4_permitted_nets)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder/0.2/get_ipv4_permitted_nets");
	}
	if (_cmds->add_handler("finder/0.2/get_ipv6_permitted_hosts",
	    callback(this, &XrlFinderTargetBase::handle_finder_0_2_get_ipv6_permitted_hosts)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder/0.2/get_ipv6_permitted_hosts");
	}
	if (_cmds->add_handler("finder/0.2/get_ipv6_permitted_nets",
	    callback(this, &XrlFinderTargetBase::handle_finder_0_2_get_ipv6_permitted_nets)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder/0.2/get_ipv6_permitted_nets");
	}
	if (_cmds->add_handler("finder_event_notifier/0.1/register_class_event_interest",
	    callback(this, &XrlFinderTargetBase::handle_finder_event_notifier_0_1_register_class_event_interest)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder_event_notifier/0.1/register_class_event_interest");
	}
	if (_cmds->add_handler("finder_event_notifier/0.1/deregister_class_event_interest",
	    callback(this, &XrlFinderTargetBase::handle_finder_event_notifier_0_1_deregister_class_event_interest)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder_event_notifier/0.1/deregister_class_event_interest");
	}
	if (_cmds->add_handler("finder_event_notifier/0.1/register_instance_event_interest",
	    callback(this, &XrlFinderTargetBase::handle_finder_event_notifier_0_1_register_instance_event_interest)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder_event_notifier/0.1/register_instance_event_interest");
	}
	if (_cmds->add_handler("finder_event_notifier/0.1/deregister_instance_event_interest",
	    callback(this, &XrlFinderTargetBase::handle_finder_event_notifier_0_1_deregister_instance_event_interest)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://%s/%s", "finder", "finder_event_notifier/0.1/deregister_instance_event_interest");
	}
	_cmds->finalize();
}

void
XrlFinderTargetBase::remove_handlers()
{
	_cmds->remove_handler("common/0.1/get_target_name");
	_cmds->remove_handler("common/0.1/get_version");
	_cmds->remove_handler("common/0.1/get_status");
	_cmds->remove_handler("common/0.1/shutdown");
	_cmds->remove_handler("finder/0.2/register_finder_client");
	_cmds->remove_handler("finder/0.2/unregister_finder_client");
	_cmds->remove_handler("finder/0.2/set_finder_client_enabled");
	_cmds->remove_handler("finder/0.2/finder_client_enabled");
	_cmds->remove_handler("finder/0.2/add_xrl");
	_cmds->remove_handler("finder/0.2/remove_xrl");
	_cmds->remove_handler("finder/0.2/resolve_xrl");
	_cmds->remove_handler("finder/0.2/get_xrl_targets");
	_cmds->remove_handler("finder/0.2/get_xrls_registered_by");
	_cmds->remove_handler("finder/0.2/get_ipv4_permitted_hosts");
	_cmds->remove_handler("finder/0.2/get_ipv4_permitted_nets");
	_cmds->remove_handler("finder/0.2/get_ipv6_permitted_hosts");
	_cmds->remove_handler("finder/0.2/get_ipv6_permitted_nets");
	_cmds->remove_handler("finder_event_notifier/0.1/register_class_event_interest");
	_cmds->remove_handler("finder_event_notifier/0.1/deregister_class_event_interest");
	_cmds->remove_handler("finder_event_notifier/0.1/register_instance_event_interest");
	_cmds->remove_handler("finder_event_notifier/0.1/deregister_instance_event_interest");
}

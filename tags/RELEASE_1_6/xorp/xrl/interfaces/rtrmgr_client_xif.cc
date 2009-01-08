/*
 * Copyright (c) 2001-2009 XORP, Inc.
 * See the XORP LICENSE.lgpl file for licensing, conditions, and warranties
 * on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'clnt-gen'.
 */

#ident "$XORP: xorp/xrl/interfaces/rtrmgr_client_xif.cc,v 1.24 2008/10/02 21:58:39 bms Exp $"

#include "rtrmgr_client_xif.hh"

bool
XrlRtrmgrClientV0p2Client::send_new_config_user(
	const char*	dst_xrl_target_name,
	const uint32_t&	userid,
	const NewConfigUserCB&	cb
)
{
    static Xrl* x = NULL;

    if (!x) {
        x = new Xrl(dst_xrl_target_name, "rtrmgr_client/0.2/new_config_user");
        x->args().add(XrlAtom(userid));
    }

    x->set_target(dst_xrl_target_name);

    x->args().set_arg(0, userid);

    return _sender->send(*x, callback(this, &XrlRtrmgrClientV0p2Client::unmarshall_new_config_user, cb));
}


/* Unmarshall new_config_user */
void
XrlRtrmgrClientV0p2Client::unmarshall_new_config_user(
	const XrlError&	e,
	XrlArgs*	a,
	NewConfigUserCB		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u)", XORP_UINT_CAST(a->size()), XORP_UINT_CAST(0));
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRtrmgrClientV0p2Client::send_config_saved_done(
	const char*	dst_xrl_target_name,
	const bool&	success,
	const string&	errmsg,
	const ConfigSavedDoneCB&	cb
)
{
    static Xrl* x = NULL;

    if (!x) {
        x = new Xrl(dst_xrl_target_name, "rtrmgr_client/0.2/config_saved_done");
        x->args().add(XrlAtom(success));
        x->args().add(XrlAtom(errmsg));
    }

    x->set_target(dst_xrl_target_name);

    x->args().set_arg(0, success);
    x->args().set_arg(1, errmsg);

    return _sender->send(*x, callback(this, &XrlRtrmgrClientV0p2Client::unmarshall_config_saved_done, cb));
}


/* Unmarshall config_saved_done */
void
XrlRtrmgrClientV0p2Client::unmarshall_config_saved_done(
	const XrlError&	e,
	XrlArgs*	a,
	ConfigSavedDoneCB		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u)", XORP_UINT_CAST(a->size()), XORP_UINT_CAST(0));
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRtrmgrClientV0p2Client::send_config_change_done(
	const char*	dst_xrl_target_name,
	const bool&	success,
	const string&	errmsg,
	const ConfigChangeDoneCB&	cb
)
{
    static Xrl* x = NULL;

    if (!x) {
        x = new Xrl(dst_xrl_target_name, "rtrmgr_client/0.2/config_change_done");
        x->args().add(XrlAtom(success));
        x->args().add(XrlAtom(errmsg));
    }

    x->set_target(dst_xrl_target_name);

    x->args().set_arg(0, success);
    x->args().set_arg(1, errmsg);

    return _sender->send(*x, callback(this, &XrlRtrmgrClientV0p2Client::unmarshall_config_change_done, cb));
}


/* Unmarshall config_change_done */
void
XrlRtrmgrClientV0p2Client::unmarshall_config_change_done(
	const XrlError&	e,
	XrlArgs*	a,
	ConfigChangeDoneCB		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u)", XORP_UINT_CAST(a->size()), XORP_UINT_CAST(0));
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRtrmgrClientV0p2Client::send_config_changed(
	const char*	dst_xrl_target_name,
	const uint32_t&	userid,
	const string&	deltas,
	const string&	deletions,
	const ConfigChangedCB&	cb
)
{
    static Xrl* x = NULL;

    if (!x) {
        x = new Xrl(dst_xrl_target_name, "rtrmgr_client/0.2/config_changed");
        x->args().add(XrlAtom(userid));
        x->args().add(XrlAtom(deltas));
        x->args().add(XrlAtom(deletions));
    }

    x->set_target(dst_xrl_target_name);

    x->args().set_arg(0, userid);
    x->args().set_arg(1, deltas);
    x->args().set_arg(2, deletions);

    return _sender->send(*x, callback(this, &XrlRtrmgrClientV0p2Client::unmarshall_config_changed, cb));
}


/* Unmarshall config_changed */
void
XrlRtrmgrClientV0p2Client::unmarshall_config_changed(
	const XrlError&	e,
	XrlArgs*	a,
	ConfigChangedCB		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u)", XORP_UINT_CAST(a->size()), XORP_UINT_CAST(0));
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRtrmgrClientV0p2Client::send_module_status(
	const char*	dst_xrl_target_name,
	const string&	modname,
	const uint32_t&	status,
	const ModuleStatusCB&	cb
)
{
    static Xrl* x = NULL;

    if (!x) {
        x = new Xrl(dst_xrl_target_name, "rtrmgr_client/0.2/module_status");
        x->args().add(XrlAtom(modname));
        x->args().add(XrlAtom(status));
    }

    x->set_target(dst_xrl_target_name);

    x->args().set_arg(0, modname);
    x->args().set_arg(1, status);

    return _sender->send(*x, callback(this, &XrlRtrmgrClientV0p2Client::unmarshall_module_status, cb));
}


/* Unmarshall module_status */
void
XrlRtrmgrClientV0p2Client::unmarshall_module_status(
	const XrlError&	e,
	XrlArgs*	a,
	ModuleStatusCB		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u)", XORP_UINT_CAST(a->size()), XORP_UINT_CAST(0));
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

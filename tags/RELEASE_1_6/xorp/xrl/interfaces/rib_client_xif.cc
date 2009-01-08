/*
 * Copyright (c) 2001-2009 XORP, Inc.
 * See the XORP LICENSE.lgpl file for licensing, conditions, and warranties
 * on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'clnt-gen'.
 */

#ident "$XORP: xorp/xrl/interfaces/rib_client_xif.cc,v 1.23 2008/10/02 21:58:39 bms Exp $"

#include "rib_client_xif.hh"

bool
XrlRibClientV0p1Client::send_route_info_changed4(
	const char*	dst_xrl_target_name,
	const IPv4&	addr,
	const uint32_t&	prefix_len,
	const IPv4&	nexthop,
	const uint32_t&	metric,
	const uint32_t&	admin_distance,
	const string&	protocol_origin,
	const RouteInfoChanged4CB&	cb
)
{
    static Xrl* x = NULL;

    if (!x) {
        x = new Xrl(dst_xrl_target_name, "rib_client/0.1/route_info_changed4");
        x->args().add(XrlAtom(addr));
        x->args().add(XrlAtom(prefix_len));
        x->args().add(XrlAtom(nexthop));
        x->args().add(XrlAtom(metric));
        x->args().add(XrlAtom(admin_distance));
        x->args().add(XrlAtom(protocol_origin));
    }

    x->set_target(dst_xrl_target_name);

    x->args().set_arg(0, addr);
    x->args().set_arg(1, prefix_len);
    x->args().set_arg(2, nexthop);
    x->args().set_arg(3, metric);
    x->args().set_arg(4, admin_distance);
    x->args().set_arg(5, protocol_origin);

    return _sender->send(*x, callback(this, &XrlRibClientV0p1Client::unmarshall_route_info_changed4, cb));
}


/* Unmarshall route_info_changed4 */
void
XrlRibClientV0p1Client::unmarshall_route_info_changed4(
	const XrlError&	e,
	XrlArgs*	a,
	RouteInfoChanged4CB		cb
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
XrlRibClientV0p1Client::send_route_info_changed6(
	const char*	dst_xrl_target_name,
	const IPv6&	addr,
	const uint32_t&	prefix_len,
	const IPv6&	nexthop,
	const uint32_t&	metric,
	const uint32_t&	admin_distance,
	const string&	protocol_origin,
	const RouteInfoChanged6CB&	cb
)
{
    static Xrl* x = NULL;

    if (!x) {
        x = new Xrl(dst_xrl_target_name, "rib_client/0.1/route_info_changed6");
        x->args().add(XrlAtom(addr));
        x->args().add(XrlAtom(prefix_len));
        x->args().add(XrlAtom(nexthop));
        x->args().add(XrlAtom(metric));
        x->args().add(XrlAtom(admin_distance));
        x->args().add(XrlAtom(protocol_origin));
    }

    x->set_target(dst_xrl_target_name);

    x->args().set_arg(0, addr);
    x->args().set_arg(1, prefix_len);
    x->args().set_arg(2, nexthop);
    x->args().set_arg(3, metric);
    x->args().set_arg(4, admin_distance);
    x->args().set_arg(5, protocol_origin);

    return _sender->send(*x, callback(this, &XrlRibClientV0p1Client::unmarshall_route_info_changed6, cb));
}


/* Unmarshall route_info_changed6 */
void
XrlRibClientV0p1Client::unmarshall_route_info_changed6(
	const XrlError&	e,
	XrlArgs*	a,
	RouteInfoChanged6CB		cb
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
XrlRibClientV0p1Client::send_route_info_invalid4(
	const char*	dst_xrl_target_name,
	const IPv4&	addr,
	const uint32_t&	prefix_len,
	const RouteInfoInvalid4CB&	cb
)
{
    static Xrl* x = NULL;

    if (!x) {
        x = new Xrl(dst_xrl_target_name, "rib_client/0.1/route_info_invalid4");
        x->args().add(XrlAtom(addr));
        x->args().add(XrlAtom(prefix_len));
    }

    x->set_target(dst_xrl_target_name);

    x->args().set_arg(0, addr);
    x->args().set_arg(1, prefix_len);

    return _sender->send(*x, callback(this, &XrlRibClientV0p1Client::unmarshall_route_info_invalid4, cb));
}


/* Unmarshall route_info_invalid4 */
void
XrlRibClientV0p1Client::unmarshall_route_info_invalid4(
	const XrlError&	e,
	XrlArgs*	a,
	RouteInfoInvalid4CB		cb
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
XrlRibClientV0p1Client::send_route_info_invalid6(
	const char*	dst_xrl_target_name,
	const IPv6&	addr,
	const uint32_t&	prefix_len,
	const RouteInfoInvalid6CB&	cb
)
{
    static Xrl* x = NULL;

    if (!x) {
        x = new Xrl(dst_xrl_target_name, "rib_client/0.1/route_info_invalid6");
        x->args().add(XrlAtom(addr));
        x->args().add(XrlAtom(prefix_len));
    }

    x->set_target(dst_xrl_target_name);

    x->args().set_arg(0, addr);
    x->args().set_arg(1, prefix_len);

    return _sender->send(*x, callback(this, &XrlRibClientV0p1Client::unmarshall_route_info_invalid6, cb));
}


/* Unmarshall route_info_invalid6 */
void
XrlRibClientV0p1Client::unmarshall_route_info_invalid6(
	const XrlError&	e,
	XrlArgs*	a,
	RouteInfoInvalid6CB		cb
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

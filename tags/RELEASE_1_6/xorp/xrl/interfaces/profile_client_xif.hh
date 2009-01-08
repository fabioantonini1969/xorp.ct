/*
 * Copyright (c) 2001-2009 XORP, Inc.
 * See the XORP LICENSE.lgpl file for licensing, conditions, and warranties
 * on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'clnt-gen'.
 *
 * $XORP: xorp/xrl/interfaces/profile_client_xif.hh,v 1.12 2008/10/02 21:58:38 bms Exp $
 */

#ifndef __XRL_INTERFACES_PROFILE_CLIENT_XIF_HH__
#define __XRL_INTERFACES_PROFILE_CLIENT_XIF_HH__

#undef XORP_LIBRARY_NAME
#define XORP_LIBRARY_NAME "XifProfileClient"

#include "libxorp/xlog.h"
#include "libxorp/callback.hh"

#include "libxipc/xrl.hh"
#include "libxipc/xrl_error.hh"
#include "libxipc/xrl_sender.hh"


class XrlProfileClientV0p1Client {
public:
    XrlProfileClientV0p1Client(XrlSender* s) : _sender(s) {}
    virtual ~XrlProfileClientV0p1Client() {}

    typedef XorpCallback1<void, const XrlError&>::RefPtr LogCB;
    /**
     *  Send Xrl intended to:
     *
     *  A log entry that is sent from a profile server.
     *
     *  @param dst_xrl_target_name the Xrl target name of the destination.
     *
     *  @param pname profiling variable.
     *
     *  @param sec absolute time in seconds that this entry was created.
     *
     *  @param usec absolute time in micro seconds that this entry was created.
     *
     *  @param comment a comment to be appended to this entry.
     */
    bool send_log(
	const char*	dst_xrl_target_name,
	const string&	pname,
	const uint32_t&	sec,
	const uint32_t&	usec,
	const string&	comment,
	const LogCB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr FinishedCB;
    /**
     *  Send Xrl intended to:
     *
     *  Sent to notify the client that all log entries have been sent.
     *
     *  @param dst_xrl_target_name the Xrl target name of the destination.
     *
     *  @param pname profiling variable.
     */
    bool send_finished(
	const char*	dst_xrl_target_name,
	const string&	pname,
	const FinishedCB&	cb
    );

protected:
    XrlSender* _sender;

private:
    void unmarshall_log(
	const XrlError&	e,
	XrlArgs*	a,
	LogCB		cb
    );

    void unmarshall_finished(
	const XrlError&	e,
	XrlArgs*	a,
	FinishedCB		cb
    );

};

#endif /* __XRL_INTERFACES_PROFILE_CLIENT_XIF_HH__ */

// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-
// vim:set sts=4 ts=8:

// Copyright (c) 2001-2008 XORP, Inc.
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

#ident "$XORP: xorp/libxipc/test_receiver.cc,v 1.2 2008/09/23 07:59:16 abittau Exp $"

#include "xrl_module.h"
#include "libxorp/xorp.h"
#include "libxorp/xlog.h"
#include "libxorp/debug.h"
#include "libxorp/xorpfd.hh"
#include "libxorp/callback.hh"
#include "libxorp/eventloop.hh"
#include "libxorp/exceptions.hh"
#include "libxorp/profile.hh"
#include "libxorp/status_codes.h"
#include "libxipc/xrl_std_router.hh"
#include "xrl/targets/test_xrls_base.hh"
#include "test_receiver.hh"

#define TIMEOUT_EXIT_MS 20000

using namespace SP;

TestReceiver::TestReceiver(EventLoop& eventloop, XrlRouter* xrl_router)
	: XrlTestXrlsTargetBase(xrl_router),
	  _eventloop(eventloop),
	  _received_xrls(0),
	  _done(false),
	  _sample(false)
{
}

TestReceiver::~TestReceiver()
{
}

bool
TestReceiver::done() const
{
    return _done;
}

void
TestReceiver::enable_sampler()
{
    _sample = true;
}

void
TestReceiver::print_xrl_received() const
{
#if PRINT_DEBUG
    printf(".");
    if (! (_received_xrls % 10000))
	printf("Received %u\n", XORP_UINT_CAST(_received_xrls));
#endif // PRINT_DEBUG
}

XrlCmdError
TestReceiver::common_0_1_get_target_name(
	// Output values,
	string&	name)
{
    name = "TestReceiver";
    return XrlCmdError::OKAY();
}

XrlCmdError
TestReceiver::common_0_1_get_version(
	// Output values,
	string&	version)
{
    version = "0.1";
    return XrlCmdError::OKAY();
}

XrlCmdError
TestReceiver::common_0_1_get_status(
	// Output values,
	uint32_t&	status,
	string&	reason)
{
    return XrlCmdError::OKAY();

    reason = "Ready";
    status = PROC_READY;
    return XrlCmdError::OKAY();
}

XrlCmdError
TestReceiver::common_0_1_shutdown()
{
    // TODO: XXX: PAVPAVPAV: implement it!
    return XrlCmdError::OKAY();
}

XrlCmdError
TestReceiver::test_xrls_0_1_start_transmission()
{
    _received_xrls = 0;
    TimerList::system_gettimeofday(&_start_time);
    return XrlCmdError::OKAY();
}

XrlCmdError
TestReceiver::test_xrls_0_1_end_transmission()
{
    TimerList::system_gettimeofday(&_end_time);
    print_statistics();
    return XrlCmdError::OKAY();
}

XrlCmdError
TestReceiver::test_xrls_0_1_add_xrl0()
{
    print_xrl_received();
    _received_xrls++;
    return XrlCmdError::OKAY();
}

XrlCmdError
TestReceiver::test_xrls_0_1_add_xrl1(
	// Input values,
	const string&	data1)
{
    print_xrl_received();
    _received_xrls++;
    return XrlCmdError::OKAY();
    UNUSED(data1);
}

XrlCmdError
TestReceiver::test_xrls_0_1_add_xrl2(
	// Input values,
	const string&	data1,
	const string&	data2)
{
    print_xrl_received();
    _received_xrls++;
    return XrlCmdError::OKAY();
    UNUSED(data1);
    UNUSED(data2);
}

XrlCmdError
TestReceiver::test_xrls_0_1_add_xrl9(
	// Input values,
	const bool&	data1,
	const int32_t&	data2,
	const IPv4&	data3,
	const IPv4Net&	data4,
	const IPv6&	data5,
	const IPv6Net&	data6,
	const Mac&	data7,
	const string&	data8,
	const vector<uint8_t>& data9)
{
    print_xrl_received();
    _received_xrls++;
    return XrlCmdError::OKAY();
    UNUSED(data1);
    UNUSED(data2);
    UNUSED(data3);
    UNUSED(data4);
    UNUSED(data5);
    UNUSED(data6);
    UNUSED(data7);
    UNUSED(data8);
    UNUSED(data9);
}

XrlCmdError
TestReceiver::test_xrls_0_1_add_xrlx(const XrlAtomList &)
{
    if (_sample)
	add_sample("XRL received");

    print_xrl_received();
    _received_xrls++;
    return XrlCmdError::OKAY();
}

void
TestReceiver::print_statistics()
{
    TimeVal delta_time = _end_time - _start_time;

    if (_received_xrls == 0) {
        printf("No XRLs received\n");
        return;
    }
    if (delta_time == TimeVal::ZERO()) {
        printf("Received %u XRLs; delta-time = %s secs\n",
	       XORP_UINT_CAST(_received_xrls), delta_time.str().c_str());
        return;
    }

    double double_time = delta_time.get_double();
    double speed = _received_xrls;
    speed /= double_time;

    printf("Received %u XRLs; delta_time = %s secs; speed = %f XRLs/s\n",
           XORP_UINT_CAST(_received_xrls),
	   delta_time.str().c_str(), speed);

#if RECEIVE_DO_EXIT
    // XXX: if enabled, then exit after all XRLs have been received.
    if (_exit_timer.scheduled() == false)
        _exit_timer = _eventloop.set_flag_after_ms(TIMEOUT_EXIT_MS, &_done);
#endif

    return;
}

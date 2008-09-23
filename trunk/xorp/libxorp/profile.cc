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

#ident "$XORP: xorp/libxorp/profile.cc,v 1.11 2008/07/23 05:10:53 pavlin Exp $"

#include "libxorp_module.h"
#include "xorp.h"

#include "libxorp/timeval.hh"
#include "libxorp/timer.hh"

#include "xlog.h"
#include "debug.h"
#include "profile.hh"

Profile::Profile()
    : _profile_cnt(0)
{
}

inline
void
zap(pair<const string, ref_ptr<Profile::ProfileState> >& p)
{
    p.second->zap();
}

Profile::~Profile()
{
    for_each(_profiles.begin(), _profiles.end(), ptr_fun(zap));
}

void
Profile::create(const string& pname, const string& comment)
    throw(PVariableExists)
{
    // Catch initialization problems.
    if (_profiles.count(pname))
	xorp_throw(PVariableExists, pname.c_str());

    ProfileState *p = new ProfileState(comment, false, false, new logentries);
    _profiles[pname] = ref_ptr<ProfileState>(p);
}

void
Profile::log(const string& pname, string comment)
    throw(PVariableUnknown,PVariableNotEnabled)
{
    profiles::iterator i = _profiles.find(pname);

    // Catch any mispelt pnames.
    if (i == _profiles.end())
	xorp_throw(PVariableUnknown, pname.c_str());

    // In order to be logging, we must be enabled.
    if (!i->second->enabled())
	xorp_throw(PVariableNotEnabled, pname.c_str());
    
#if	0
    // Make sure that this variable is not locked.
    if (!i->second->locked())
	xorp_throw(PVariableLocked, pname.c_str());
#endif
    
    TimeVal tv;
    TimerList::system_gettimeofday(&tv);
    i->second->logptr()->push_back(ProfileLogEntry(tv, comment));
}

void
Profile::enable(const string& pname) throw(PVariableUnknown,PVariableLocked)
{
    profiles::iterator i = _profiles.find(pname);

    // Catch any mispelt pnames.
    if (i == _profiles.end())
	xorp_throw(PVariableUnknown, pname.c_str());

    // If this profile name is already enabled, get out of here
    // without updating the counter.
    if (i->second->enabled())
	return;

    // Don't allow a locked entry to be enabled.
    if (i->second->locked())
	xorp_throw(PVariableLocked, pname.c_str());
    
    i->second->set_enabled(true);
    _profile_cnt++;
}

void
Profile::disable(const string& pname) throw(PVariableUnknown)
{
    profiles::iterator i = _profiles.find(pname);

    // Catch any mispelt pnames.
    if (i == _profiles.end())
	xorp_throw(PVariableUnknown, pname.c_str());

    // If this profile name is already disabled, get out of here
    // without updating the counter.
    if (!i->second->enabled())
	return;
    i->second->set_enabled(false);
    _profile_cnt--;
}

void
Profile::lock_log(const string& pname) throw(PVariableUnknown,PVariableLocked)
{
    profiles::iterator i = _profiles.find(pname);

    // Catch any mispelt pnames.
    if (i == _profiles.end())
	xorp_throw(PVariableUnknown, pname.c_str());

    // Don't allow a locked entry to be locked again.
    if (i->second->locked())
	xorp_throw(PVariableLocked, pname.c_str());

    // Disable logging.
    disable(pname);

    // Lock the entry
    i->second->set_locked(true);

    i->second->set_iterator(i->second->logptr()->begin());
}

bool 
Profile::read_log(const string& pname, ProfileLogEntry& entry) 
    throw(PVariableUnknown,PVariableNotLocked)
{
    profiles::iterator i = _profiles.find(pname);

    // Catch any mispelt pnames.
    if (i == _profiles.end())
	xorp_throw(PVariableUnknown, pname.c_str());

    // Verify that the log entry is locked
    if (!i->second->locked())
	xorp_throw(PVariableNotLocked, pname.c_str());

    logentries::iterator li;
    i->second->get_iterator(li);
    if (li == i->second->logptr()->end())
	return false;

    entry = *li;
    i->second->set_iterator(++li);

    return true;
}

void
Profile::release_log(const string& pname) 
    throw(PVariableUnknown,PVariableNotLocked)
{
    profiles::iterator i = _profiles.find(pname);

    // Catch any mispelt pnames.
    if (i == _profiles.end())
	xorp_throw(PVariableUnknown, pname.c_str());

    // Verify that the log entry is locked
    if (!i->second->locked())
	xorp_throw(PVariableNotLocked, pname.c_str());

    // Unlock the entry
    i->second->set_locked(false);
}

void
Profile::clear(const string& pname) throw(PVariableUnknown,PVariableLocked)
{
    profiles::iterator i = _profiles.find(pname);

    // Catch any mispelt pnames.
    if (i == _profiles.end())
	xorp_throw(PVariableUnknown, pname.c_str());

    // Don't allow a locked entry to be cleared.
    if (i->second->locked())
	xorp_throw(PVariableLocked, pname.c_str());

    i->second->logptr()->clear();
}

class List: public unary_function<pair<const string, 
				       ref_ptr<Profile::ProfileState> >,
				  void> {
 public:
    void operator()(const pair<const string, ref_ptr<Profile::ProfileState> >& p) {
	_result += p.first;
	_result += "\t";
	_result += c_format("%d", p.second->size());
	_result += "\t";
	_result += p.second->enabled() ? "enabled" : "disabled";
	_result += "\t";
	_result += p.second->comment();
	_result += "\n";
    }

    string result() const {
	return _result;
    }
 private:
    string _result;
};

string
Profile::list() const
{
    return for_each(_profiles.begin(), _profiles.end(), List()).result();
}

// simple profiler
SP::SAMPLE
SP::sampler_time()
{
    TimeVal tv;

    TimerList::system_gettimeofday(&tv);

    SAMPLE ret = tv.secs();

    ret *= (SAMPLE) 1000000;
    ret += (SAMPLE) tv.usec();

    return ret;
}

#if defined(__i386__) && defined(__GNUC__)
// XXX watch out on SMP systems - make sure u're always reading the same tsc
// (i.e., same core running the process).  Or disable smp.
// -sorbo.
SP::SAMPLE
SP::sampler_tsc(void)
{   
    uint64_t tsc;

    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (tsc));

    return tsc;
}

SP::SAMPLER SP::_sampler = SP::sampler_tsc;
#else
SP::SAMPLER SP::_sampler = SP::sampler_time;
#endif // i386 && GNUC

namespace SP {
    SAMPLE      _samples[SP_MAX_SAMPLES];
    const char* _desc[SP_MAX_SAMPLES];
    unsigned    _samplec;
}

void
SP::set_sampler(SAMPLER sampler)
{
    _sampler = sampler;
}

void
SP::add_sample(const char* desc)
{
    if (!_sampler)
	return;

    XLOG_ASSERT(_samplec < SP_MAX_SAMPLES);

    _samples[_samplec] = _sampler();
    _desc[_samplec]    = desc;

    _samplec++;
}

void
SP::print_samples()
{
    if (!_samplec)
	return;

    double total = _samples[_samplec - 1] - _samples[0];

    printf("\n");
    printf("Absolute time\tElapsed time\tPercentage\tDescription\n");

    for (unsigned i = 0; i < _samplec; i++) {
        printf("%llu\t", _samples[i]);

        if (i != 0) {
            SAMPLE a, b, diff;

            a = _samples[i - 1];
            b = _samples[i];

            XLOG_ASSERT(a <= b);

            diff = b - a;

            printf("%12llu\t%10.2f\t", diff, (double) diff / total * 100.0);
        } else
            printf("\t\t\t\t");

        printf("%s\n", _desc[i]);
    }

    printf("Total %llu\n", (SAMPLE) total);
    printf("\n");

    _samplec = 0;
}

SP::SAMPLE
SP::sample()
{
    if (_sampler)
	return _sampler();

    return 0;
}

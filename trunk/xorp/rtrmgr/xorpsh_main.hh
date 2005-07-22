// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2005 International Computer Science Institute
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

// $XORP: xorp/rtrmgr/xorpsh_main.hh,v 1.25 2005/07/15 06:39:08 pavlin Exp $

#ifndef __RTRMGR_XORPSH_MAIN_HH__
#define __RTRMGR_XORPSH_MAIN_HH__


#include "libxorp/eventloop.hh"

#include "libxipc/xrl_std_router.hh"

#include "cli/cli_node.hh"

#include "xrl/interfaces/rtrmgr_xif.hh"

#include "rtrmgr_error.hh"
#include "xorp_client.hh"
#include "xrl_xorpsh_interface.hh"
#include "slave_module_manager.hh"
#include "xorpsh_base.hh"

class OpCommandList;
class RouterCLI;
class SlaveConfigTree;
class TemplateTree;

class XorpShell : XorpShellBase {
public:
    XorpShell(const string& IPCname, 
	      const string& xorp_root_dir,
	      const string& config_template_dir, 
	      const string& xrl_targets_dir,
	      bool verbose) throw (InitError);
    ~XorpShell();

    void run(const string& command);
    bool done() const;


    void set_mode(Mode mode) { _mode = mode; }
    
    void register_done(const XrlError& e, const string* token,
		       const uint32_t* pid, const uint32_t* clientid);
    void generic_done(const XrlError& e);
    void request_config();
    void receive_config(const XrlError& e, const bool* ready,
			const string* config);

    void enter_config_mode(bool exclusive, GENERIC_CALLBACK cb);

    void leave_config_mode(GENERIC_CALLBACK cb);

    void lock_config(LOCK_CALLBACK cb);

    void config_saved_done(bool success, const string& errmsg);
    void commit_changes(const string& deltas, const string& deletions,
			GENERIC_CALLBACK cb,
			CallBack final_cb);
    void commit_done(bool success, const string& errmsg);

    void unlock_config(GENERIC_CALLBACK cb);

    void get_config_users(GET_USERS_CALLBACK cb);

    void new_config_user(uid_t user_id);

    void save_to_file(const string& filename, GENERIC_CALLBACK cb,
			CallBack final_cb);

    void save_lock_achieved(const XrlError& e, const bool* locked,
			    const uint32_t* lock_holder,
			    const string filename,
			    GENERIC_CALLBACK cb);

    void load_from_file(const string& filename, GENERIC_CALLBACK cb,
			CallBack final_cb);

    void load_lock_achieved(const XrlError& e, const bool* locked,
			    const uint32_t* lock_holder,
			    const string filename,
			    GENERIC_CALLBACK cb);

    void config_changed(uid_t user_id, const string& deltas, 
			const string& deletions);

    void module_status_change(const string& modname, 
			      GenericModule::ModuleStatus status);

    void get_rtrmgr_pid(PID_CALLBACK cb);

    EventLoop& eventloop()		{ return _eventloop; }
    OpCommandList* op_cmd_list()	{ return _ocl; }
    SlaveConfigTree* config_tree()	{ return _ct; }
    TemplateTree* template_tree()	{ return _tt; }
    uint32_t clientid() const		{ return _clientid; }
    uint32_t rtrmgr_pid() const		{ return _rtrmgr_pid; }
    XorpClient& xorp_client()		{ return _xclient; }
    const string& xorp_root_dir() const	{ return _xorp_root_dir; }
    bool verbose() const		{ return _verbose; }

private:
    EventLoop		_eventloop; 
    XrlStdRouter	_xrlrouter;
    XorpClient		_xclient;
    XrlRtrmgrV0p1Client	_rtrmgr_client;
    SlaveModuleManager	_mmgr;

    TemplateTree*	_tt;
    SlaveConfigTree*	_ct;
    OpCommandList*	_ocl;
    CliNode		_cli_node;
    RouterCLI*		_router_cli;
    string		_xorp_root_dir;	// The root of the XORP tree
    bool		_verbose;	// Set to true if output is verbose
    string		_ipc_name;
    string		_authfile;
    string		_authtoken;
    bool		_got_config;
    bool		_got_modules;
    string		_configuration;

    bool		_xrl_generic_done;	// XRL startup/shutdown flag
    Mode		_mode;

    // Used to store the callback during a commit until we get called
    // with the response
    CallBack		_commit_callback;
    string		_commit_status;	// Used for transient storage of error
					// messages from commit

    // Used to store the callback during saving a file until we get called
    // with the response
    CallBack		_config_save_callback;
    string		_save_status;	// Used for transient storage of error
					// messages from saving the config

    uint32_t		_rtrmgr_pid;
    uint32_t            _clientid;

    XorpTimer           _repeat_request_timer;

    int			_fddesc[2];

    // XXX: must be last
    XrlXorpshInterface	_xorpsh_interface;
};

#endif // __RTRMGR_XORPSH_MAIN_HH__

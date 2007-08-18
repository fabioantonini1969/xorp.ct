// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2007 International Computer Science Institute
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

// $XORP: xorp/fea/data_plane/fibconfig/fibconfig_entry_set_rtmv2.hh,v 1.2 2007/07/11 22:18:08 pavlin Exp $

#ifndef __FEA_DATA_PLANE_FIBCONFIG_FIBCONFIG_ENTRY_SET_RTMV2_HH__
#define __FEA_DATA_PLANE_FIBCONFIG_FIBCONFIG_ENTRY_SET_RTMV2_HH__

#include "fea/fibconfig_entry_set.hh"

class WinRtmPipe;


class FibConfigEntrySetRtmV2 : public FibConfigEntrySet {
public:
    /**
     * Constructor.
     *
     * @param fea_data_plane_manager the corresponding data plane manager
     * (@see FeaDataPlaneManager).
     */
    FibConfigEntrySetRtmV2(FeaDataPlaneManager& fea_data_plane_manager);

    /**
     * Virtual destructor.
     */
    virtual ~FibConfigEntrySetRtmV2();

    /**
     * Start operation.
     *
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int start(string& error_msg);

    /**
     * Stop operation.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int stop(string& error_msg);

    /**
     * Add a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry4(const Fte4& fte);

    /**
     * Delete a single routing entry. Must be with a configuration interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry4(const Fte4& fte);

    /**
     * Add a single routing entry. Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry6(const Fte6& fte);

    /**
     * Delete a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry6(const Fte6& fte);

private:
    WinRtmPipe*		_rs4;
    WinRtmPipe*		_rs6;
    bool add_entry(const FteX& fte);
    bool delete_entry(const FteX& fte);
};

#endif // __FEA_DATA_PLANE_FIBCONFIG_FIBCONFIG_ENTRY_SET_RTMV2_HH__
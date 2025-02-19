/*****************************************************************************
**
**  Name:           bta_dm_int.h
**
**  Description:    This is the private interface file for the BTA device
**                  manager.
**
**  Copyright (c) 2003-2015, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef BTA_DM_INT_H
#define BTA_DM_INT_H

#include "bt_target.h"
#include "bta_sys.h"

#if (BLE_INCLUDED == TRUE && (defined BTA_GATT_INCLUDED) && (BTA_GATT_INCLUDED == TRUE))
    #include "bta_gatt_api.h"
#endif

#if (BRCM_LPST_INCLUDED == TRUE)
#include "btm_lpst_api.h"
#endif

/*****************************************************************************
**  Constants and data types
*****************************************************************************/


#define BTA_COPY_DEVICE_CLASS(coddst, codsrc)   {((UINT8 *)(coddst))[0] = ((UINT8 *)(codsrc))[0]; \
                                                 ((UINT8 *)(coddst))[1] = ((UINT8 *)(codsrc))[1]; \
                                                 ((UINT8 *)(coddst))[2] = ((UINT8 *)(codsrc))[2];}


#define BTA_DM_MSG_LEN 50

#define BTA_SERVICE_ID_TO_SERVICE_MASK(id)       (1 << (id))

/* DM events */
enum
{
    /* device manager local device API events */
    BTA_DM_API_ENABLE_EVT = BTA_SYS_EVT_START(BTA_ID_DM),
    BTA_DM_API_DISABLE_EVT,
    BTA_DM_API_SET_NAME_EVT,
    BTA_DM_API_SET_VISIBILITY_EVT,
    BTA_DM_API_SET_SCAN_CONFIG,
    BTA_DM_API_SET_AFH_CHANNELS_EVT,
    BTA_API_DM_SIG_STRENGTH_EVT,
    BTA_DM_API_VENDOR_SPECIFIC_COMMAND_EVT,
    BTA_DM_API_TX_INQPWR_EVT,
    BTA_DM_ACL_CHANGE_EVT,
    BTA_DM_API_ADD_DEVICE_EVT,
    BTA_DM_API_REMOVE_ACL_EVT,

    /* security API events */
    BTA_DM_API_BOND_EVT,
    BTA_DM_API_BOND_CANCEL_EVT,
    BTA_DM_API_PIN_REPLY_EVT,
    BTA_DM_API_LINK_POLICY_EVT,
    BTA_DM_API_AUTH_REPLY_EVT,

    /* power manger events */
    BTA_DM_PM_BTM_STATUS_EVT,
    BTA_DM_PM_TIMER_EVT,

    /* simple pairing events */
    BTA_DM_API_CONFIRM_EVT,

    BTA_DM_API_SET_ENCRYPTION_EVT,
    BTA_DM_API_READ_REMOTE_NAME_EVT,
    BTA_DM_API_SET_LOCAL_DEV_ADDR_EVT,

#if (BTM_LOCAL_IO_CAPS != BTM_IO_CAP_NONE)
    BTA_DM_API_PASKY_CANCEL_EVT,
#endif
#if (BTM_OOB_INCLUDED == TRUE)
    BTA_DM_API_LOC_OOB_EVT,
    BTA_DM_CI_IO_REQ_EVT,
    BTA_DM_CI_RMT_OOB_EVT,
#if (BTM_BR_SC_INCLUDED == TRUE)
    BTA_DM_CI_RMT_OOB_EXT_EVT,
#endif /* BTM_BR_SC_INCLUDED */
#endif /* BTM_OOB_INCLUDED */

    /* dual stack event */
#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
    BTA_DM_API_SWITCH_STACK_EVT,        /* Event when App initiates BTA_DmSwitchStack() */
    BTA_DM_IPC_EVT,                     /* Event when received IPC message */
#endif

#if BLE_INCLUDED == TRUE
    BTA_DM_API_BLE_ENABLE_EVT,
    BTA_DM_API_ADD_BLEKEY_EVT,
    BTA_DM_API_ADD_BLEDEVICE_EVT,
    BTA_DM_API_BLE_PASSKEY_REPLY_EVT,
#if SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
    BTA_DM_API_BLE_CONFIRM_REPLY_EVT,
#endif
    BTA_DM_API_BLE_SEC_GRANT_EVT,
    BTA_DM_API_BLE_SET_BG_CONN_TYPE,
    BTA_DM_API_BLE_CONN_PARAM_EVT,
    BTA_DM_API_BLE_CONN_SCAN_PARAM_EVT,
    BTA_DM_API_BLE_SCAN_PARAM_EVT,
    BTA_DM_API_BLE_OBSERVE_EVT,
    BTA_DM_API_UPDATE_CONN_PARAM_EVT,
#if BLE_BRCM_INCLUDED == TRUE && BTM_BLE_PRIVACY_SPT == TRUE
    BTA_DM_API_LOCAL_PRIVACY_EVT,
#endif
    BTA_DM_API_BLE_ADV_PARAM_EVT,
    BTA_DM_API_BLE_ADV_PARAM_All_EVT,
    BTA_DM_API_BLE_SET_ADV_CONFIG_EVT,
    /* Add for set raw advertising data */
    BTA_DM_API_BLE_SET_ADV_CONFIG_RAW_EVT,
    BTA_DM_API_BLE_BROADCAST_EVT,
#if BLE_DATA_LEN_EXT_INCLUDED == TRUE
    BTA_DM_API_SET_DATA_LEN_EVT,
#endif

#if BLE_BRCM_INCLUDED == TRUE
    BTA_DM_API_RSSI_LINK_EVT,
    BTA_DM_API_RSSI_ADV_EVT,
    //BTA_DM_API_CFG_FILTER_COND_EVT,
    //BTA_DM_API_ENABLE_ADV_FILTER_EVT,
    BTA_DM_API_BLE_FEATURE_EVT,
    BTA_DM_API_BLE_MULTI_ADV_ENB_EVT,
    BTA_DM_API_BLE_MULTI_ADV_PARAM_UPD_EVT,
    BTA_DM_API_BLE_MULTI_ADV_DATA_EVT,
    BTA_DM_API_BLE_MULTI_ADV_DISABLE_EVT,
#endif
    BTA_DM_API_BLE_SETUP_STORAGE_EVT,
    BTA_DM_API_BLE_ENABLE_BATCH_SCAN_EVT,
    BTA_DM_API_BLE_DISABLE_BATCH_SCAN_EVT,
    BTA_DM_API_BLE_READ_SCAN_REPORTS_EVT,
    BTA_DM_API_BLE_TRACK_ADVERTISER_EVT,
    BTA_DM_API_BLE_ENERGY_INFO_EVT,
    BTA_DM_API_BLE_READ_CONN_PHY_EVT,
    BTA_DM_API_BLE_SET_CONN_PHY_EVT,
    BTA_DM_API_BLE_CREATE_CONN_EVT,
#if BLE_SCAN_FILTER == TRUE
    BTA_DM_API_CFG_FILTER_COND_EVT,
    BTA_DM_API_SCAN_FILTER_SETUP_EVT,
    BTA_DM_API_SCAN_FILTER_ENABLE_EVT,
#endif
#endif

#if ( BTM_EIR_SERVER_INCLUDED == TRUE )&&( BTA_EIR_CANNED_UUID_LIST != TRUE )&&(BTA_EIR_SERVER_NUM_CUSTOM_UUID > 0)
    BTA_DM_API_UPDATE_EIR_UUID_EVT,
#endif
#if (BTM_EIR_SERVER_INCLUDED == TRUE)
    BTA_DM_API_SET_EIR_CONFIG_EVT,
#endif
#if (defined BTM_TBFC_INCLUDED) && (BTM_TBFC_INCLUDED == TRUE)
    BTA_DM_API_SET_TBFC_MODE_EVT,
#endif
    BTA_DM_API_ENABLE_TEST_MODE_EVT,
    BTA_DM_API_DISABLE_TEST_MODE_EVT,
    BTA_DM_API_EXECUTE_CBACK_EVT,
    BTA_DM_API_REMOVE_ALL_ACL_EVT,
    BTA_DM_API_REMOVE_DEVICE_EVT,
    BTA_DM_API_COEX_OP_EVT,
    BTA_DM_API_REG_COEX_CBACK_EVT,
    BTA_DM_API_PASSKEY_REQ_REPLY_EVT,

    /* consumer feature */
    BTA_DM_API_SET_NAME_LE_EVT,

    BTA_DM_MAX_EVT
};


/* DM search events */
enum
{
    /* DM search API events */
    BTA_DM_API_SEARCH_EVT = BTA_SYS_EVT_START(BTA_ID_DM_SEARCH),
    BTA_DM_API_SEARCH_CANCEL_EVT,
    BTA_DM_API_DISCOVER_EVT,
    BTA_DM_INQUIRY_CMPL_EVT,
    BTA_DM_REMT_NAME_EVT,
    BTA_DM_SDP_RESULT_EVT,
    BTA_DM_SEARCH_CMPL_EVT,
    BTA_DM_DISCOVERY_RESULT_EVT,
    BTA_DM_API_DI_DISCOVER_EVT
#if BLE_INCLUDED == TRUE
    ,BTA_DM_DISC_CLOSE_TOUT_EVT
#endif
#if (defined(BTA_MPS_INCLUDED) && BTA_MPS_INCLUDED == TRUE)
   ,BTA_DM_API_MPS_SET_RECORD_EVT,
    BTA_DM_API_MPS_DISCOVER_EVT
#endif
};

/* data type for BTA_DM_API_ENABLE_EVT */
typedef struct
{
    BT_HDR              hdr;
    tBTA_DM_SEC_CBACK *p_sec_cback;
} tBTA_DM_API_ENABLE;

/* data type for BTA_DM_API_SET_NAME_EVT */
typedef struct
{
    BT_HDR              hdr;
    BD_NAME             name; /* max 248 bytes name, plus must be Null terminated */
} tBTA_DM_API_SET_NAME;

/* data type for BTA_DM_API_SET_VISIBILITY_EVT */
typedef struct
{
    BT_HDR              hdr;
    tBTA_DM_DISC    disc_mode;
    tBTA_DM_CONN    conn_mode;
    UINT8           pair_mode;
    UINT8           conn_paired_only;
} tBTA_DM_API_SET_VISIBILITY;

/* data type for BTA_DM_API_SET_SCAN_CONFIG */
typedef struct
{
    BT_HDR              hdr;
    UINT16 page_scan_interval;
    UINT16 page_scan_window;
    UINT16 inquiry_scan_interval;
    UINT16 inquiry_scan_window;
}tBTA_DM_API_SET_SCAN_CONFIG;

/* data type for BTA_DM_API_SET_AFH_CHANNELS_EVT */
typedef struct
{
    BT_HDR              hdr;
    UINT8           first;
    UINT8           last;
} tBTA_DM_API_SET_AFH_CHANNELS_EVT;

/* data type for BTA_DM_API_VENDOR_SPECIFIC_COMMAND_EVT */
typedef struct
{
    BT_HDR              hdr;
    UINT16              opcode;
    UINT8               param_len;
    UINT8               *p_param_buf;
    tBTA_VENDOR_CMPL_CBACK *p_cback;

} tBTA_DM_API_VENDOR_SPECIFIC_COMMAND;

enum
{
    BTA_DM_RS_NONE,     /* straight API call */
    BTA_DM_RS_OK,       /* the role switch result - successful */
    BTA_DM_RS_FAIL      /* the role switch result - failed */
};
typedef UINT8 tBTA_DM_RS_RES;

/* data type for BTA_DM_API_SEARCH_EVT */
typedef struct
{
    BT_HDR      hdr;
    tBTA_DM_INQ inq_params;
    tBTA_SERVICE_MASK services;
    tBTA_DM_SEARCH_CBACK * p_cback;
    tBTA_DM_RS_RES  rs_res;
#if BLE_INCLUDED == TRUE && BTA_GATT_INCLUDED == TRUE
    UINT8           num_uuid;
    tBT_UUID        *p_uuid;
#endif
} tBTA_DM_API_SEARCH;

/* data type for BTA_DM_API_DISCOVER_EVT */
typedef struct
{
    BT_HDR      hdr;
    BD_ADDR bd_addr;
    tBTA_SERVICE_MASK services;
    tBTA_DM_SEARCH_CBACK * p_cback;
    BOOLEAN         sdp_search;
    tBTA_TRANSPORT  transport;
#if BLE_INCLUDED == TRUE && BTA_GATT_INCLUDED == TRUE
    UINT8           num_uuid;
    tBT_UUID        *p_uuid;
#endif

} tBTA_DM_API_DISCOVER;

/* data type for BTA_DM_API_DI_DISC_EVT */
typedef struct
{
    BT_HDR              hdr;
    BD_ADDR             bd_addr;
    tBTA_DISCOVERY_DB   *p_sdp_db;
    UINT32              len;
    tBTA_DM_SEARCH_CBACK * p_cback;
}tBTA_DM_API_DI_DISC;

/* data type for BTA_DM_API_BOND_EVT */
typedef struct
{
    BT_HDR      hdr;
    BD_ADDR bd_addr;
    tBTA_TRANSPORT transport;
} tBTA_DM_API_BOND;

/* data type for BTA_DM_API_BOND_CANCEL_EVT */
typedef struct
{
    BT_HDR          hdr;
    BD_ADDR         bd_addr;
    tBTA_TRANSPORT  transport;
} tBTA_DM_API_BOND_CANCEL;

/* data type for BTA_DM_API_PIN_REPLY_EVT */
typedef struct
{
    BT_HDR      hdr;
    BD_ADDR bd_addr;
    BOOLEAN accept;
    UINT8 pin_len;
    UINT8 p_pin[PIN_CODE_LEN];
} tBTA_DM_API_PIN_REPLY;

/* data type for BTA_DM_API_LINK_POLICY_EVT */
typedef struct
{
    BT_HDR      hdr;
    BD_ADDR     bd_addr;
    UINT16      policy_mask;
    BOOLEAN     set;
} tBTA_DM_API_LINK_POLICY;

/* data type for BTA_DM_API_AUTH_REPLY_EVT */
typedef struct
{
    BT_HDR      hdr;
    BD_ADDR bd_addr;
    tBTA_SERVICE_ID service;
    tBTA_AUTH_RESP response;
} tBTA_DM_API_AUTH_REPLY;

/* data type for BTA_DM_API_LOC_OOB_EVT */
typedef struct
{
    BT_HDR      hdr;
} tBTA_DM_API_LOC_OOB;

/* data type for BTA_DM_API_CONFIRM_EVT */
typedef struct
{
    BT_HDR      hdr;
    BD_ADDR     bd_addr;
    BOOLEAN     accept;
} tBTA_DM_API_CONFIRM;

/* data type for BTA_DM_API_PASKY_CANCEL_EVT*/
typedef struct
{
    BT_HDR      hdr;
    BD_ADDR     bd_addr;
} tBTA_DM_API_PASKY_CANCEL;

/* data type for BTA_DM_CI_IO_REQ_EVT */
typedef struct
{
    BT_HDR          hdr;
    BD_ADDR         bd_addr;
    tBTA_IO_CAP     io_cap;
    tBTA_OOB_DATA   oob_data;
    tBTA_AUTH_REQ   auth_req;
} tBTA_DM_CI_IO_REQ;

/* data type for BTA_DM_CI_RMT_OOB_EVT */
typedef struct
{
    BT_HDR      hdr;
    BD_ADDR     bd_addr;
    BT_OCTET16  c;
    BT_OCTET16  r;
    BOOLEAN     accept;
} tBTA_DM_CI_RMT_OOB;

#if (BTM_BR_SC_INCLUDED == TRUE)
/* data type for BTA_DM_CI_RMT_OOB_EXT_EVT */
typedef struct
{
    BT_HDR      hdr;
    BD_ADDR     bd_addr;
    BT_OCTET16  c_192;
    BT_OCTET16  r_192;
    BT_OCTET16  c_256;
    BT_OCTET16  r_256;
    BOOLEAN     accept;
} tBTA_DM_CI_RMT_OOB_EXT;
#endif

/* data type for BTA_DM_REMT_NAME_EVT */
typedef struct
{
    BT_HDR      hdr;
    tBTA_DM_SEARCH  result;
} tBTA_DM_REM_NAME;

/* data type for tBTA_DM_DISC_RESULT */
typedef struct
{
    BT_HDR      hdr;
    tBTA_DM_SEARCH  result;
} tBTA_DM_DISC_RESULT;


/* data type for BTA_DM_INQUIRY_CMPL_EVT */
typedef struct
{
    BT_HDR      hdr;
    UINT8       num;
} tBTA_DM_INQUIRY_CMPL;

/* data type for BTA_DM_SDP_RESULT_EVT */
typedef struct
{
    BT_HDR      hdr;
    UINT16 sdp_result;
} tBTA_DM_SDP_RESULT;

/* data type for BTA_API_DM_SIG_STRENGTH_EVT */
typedef struct
{
    BT_HDR      hdr;
    tBTA_SIG_STRENGTH_MASK mask;
    UINT16 period;
    BOOLEAN start;
} tBTA_API_DM_SIG_STRENGTH;

/* data type for tBTA_API_DM_TX_INQPWR */
typedef struct
{
    BT_HDR      hdr;
    INT8        tx_power;
}tBTA_API_DM_TX_INQPWR;

/* data type for BTA_DM_ACL_CHANGE_EVT */
typedef struct
{
    BT_HDR          hdr;
    tBTM_BL_EVENT   event;
    UINT8           busy_level;
    BOOLEAN         is_new;
    UINT8           new_role;
    BD_ADDR         bd_addr;
    UINT8           hci_status;
#if BLE_INCLUDED == TRUE
    UINT16          handle;
    tBT_TRANSPORT   transport;
#endif
} tBTA_DM_ACL_CHANGE;

/* data type for BTA_DM_PM_BTM_STATUS_EVT */
typedef struct
{

    BT_HDR          hdr;
    BD_ADDR         bd_addr;
    tBTM_PM_STATUS  status;
    UINT16          value;
    UINT8           hci_status;

} tBTA_DM_PM_BTM_STATUS;

/* data type for BTA_DM_PM_TIMER_EVT */
typedef struct
{
    BT_HDR              hdr;
    BD_ADDR             bd_addr;
    tBTA_DM_PM_ACTION   pm_request;
} tBTA_DM_PM_TIMER;


/* data type for BTA_DM_API_ADD_DEVICE_EVT */
typedef struct
{
    BT_HDR              hdr;
    BD_ADDR             bd_addr;
    DEV_CLASS           dc;
    LINK_KEY            link_key;
    tBTA_SERVICE_MASK   tm;
    BOOLEAN             is_trusted;
    UINT8               key_type;
    tBTA_IO_CAP         io_cap;
    BOOLEAN             link_key_known;
    BOOLEAN             dc_known;
    BD_NAME             bd_name;
    UINT8               features[BTA_FEATURE_BYTES_PER_PAGE * (BTA_EXT_FEATURES_PAGE_MAX + 1)];
} tBTA_DM_API_ADD_DEVICE;

/* data type for BTA_DM_API_REMOVE_ACL_EVT */
typedef struct {
    BT_HDR              hdr;
    BD_ADDR             bd_addr;
    UINT8               transport;
} tBTA_DM_API_REMOVE_DEVICE;

#if (defined BTM_TBFC_INCLUDED) && (BTM_TBFC_INCLUDED == TRUE)
/* data for BTA_DM_API_SET_TBFC_MODE_EVT */
typedef struct
{
    BT_HDR                  hdr;
    BD_ADDR                 bd_addr;
    tBTA_DM_TBFC_SCAN_MODE  mode;
} tBTA_DM_API_SET_TBFC_MODE;
#endif
/* data type for BTA_DM_API_EXECUTE_CBACK_EVT */
typedef struct
{
    BT_HDR               hdr;
    void *               p_param;
    tBTA_DM_EXEC_CBACK  *p_exec_cback;
} tBTA_DM_API_EXECUTE_CBACK;

#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
/* data type for BTA_DM_API_SWITCH_STACK_EVT */
typedef struct
{
    BT_HDR          hdr;
    tBTA_DM_SW_DIR  dir;
    tBTA_DM_SWITCH_CBACK *p_cback;
}tBTA_DM_API_SWITCH_STACK;
#endif


/* data type for tBTA_DM_API_SET_ENCRYPTION */
typedef struct
{
    BT_HDR                    hdr;
    tBTA_TRANSPORT            transport;
    tBTA_DM_ENCRYPT_CBACK     *p_callback;
    tBTA_DM_BLE_SEC_ACT       sec_act;
    BD_ADDR                   bd_addr;
} tBTA_DM_API_SET_ENCRYPTION;

typedef struct
{
   BT_HDR                    hdr;
   BD_ADDR                   bd_addr;
   tBTA_TRANSPORT            transport;
   tBTA_REMNAME_CMPL_CBACK  *p_cback;
}tBTA_DM_API_READ_REMOTE_NAME;

typedef struct
{
   BT_HDR                    hdr;
   BD_ADDR                   bd_addr;
   tBTA_SET_ADDR_CMPL_CBACK  *p_cback;
}tBTA_DM_API_SET_LOCAL_ADDR;

#if BLE_INCLUDED == TRUE
typedef struct
{
    BT_HDR                  hdr;
    BOOLEAN             enable;
}tBTA_DM_API_BLE_ENABLE;

typedef struct
{
    BT_HDR                  hdr;
    BD_ADDR                 bd_addr;
    tBTA_LE_KEY_VALUE       blekey;
    tBTA_LE_KEY_TYPE        key_type;

}tBTA_DM_API_ADD_BLEKEY;

typedef struct
{
    BT_HDR                  hdr;
    BD_ADDR                 bd_addr;
    tBT_DEVICE_TYPE         dev_type ;
    UINT32                  auth_mode;
    tBLE_ADDR_TYPE          addr_type;

}tBTA_DM_API_ADD_BLE_DEVICE;

typedef struct
{
    BT_HDR                  hdr;
    BD_ADDR                 bd_addr;
    BOOLEAN                 accept;
    UINT32                  passkey;
}tBTA_DM_API_PASSKEY_REPLY;

typedef struct
{
    BT_HDR                  hdr;
    BD_ADDR                 bd_addr;
    tBTA_DM_BLE_SEC_GRANT   res;
}tBTA_DM_API_BLE_SEC_GRANT;


typedef struct
{
    BT_HDR                  hdr;
    tBTA_DM_BLE_CONN_TYPE   bg_conn_type;
    tBTA_DM_BLE_SEL_CBACK   *p_select_cback;
}tBTA_DM_API_BLE_SET_BG_CONN_TYPE;

/* set prefered BLE connection parameters for a device */
typedef struct
{
    BT_HDR                  hdr;
    BD_ADDR                 peer_bda;
    UINT16                  conn_int_min;
    UINT16                  conn_int_max;
    UINT16                  supervision_tout;
    UINT16                  slave_latency;

}tBTA_DM_API_BLE_CONN_PARAMS;

typedef struct
{
    BT_HDR                  hdr;
    BD_ADDR                 peer_bda;
    BOOLEAN                 privacy_enable;

}tBTA_DM_API_ENABLE_PRIVACY;

typedef struct
{
    BT_HDR                  hdr;
    BOOLEAN                 privacy_enable;
    tBTA_SET_LOCAL_PRIVACY_CBACK *set_local_privacy_cback;
}tBTA_DM_API_LOCAL_PRIVACY;

/* set scan parameter for BLE connections */
typedef struct
{
    BT_HDR                  hdr;
    tBTA_GATTC_IF           client_if;
    UINT16                  scan_int;
    UINT16                  scan_window;
    tBLE_SCAN_MODE scan_mode;
    tBLE_SCAN_PARAM_SETUP_CBACK scan_param_setup_cback;
}tBTA_DM_API_BLE_SCAN_PARAMS;
typedef struct
{
    BT_HDR                  hdr;
    UINT16                  scan_int;
    UINT16                  scan_window;
} tBTA_DM_API_BLE_CONN_SCAN_PARAMS;

/* Data type for start/stop observe */
typedef struct {
    BT_HDR                  hdr;
    BOOLEAN                 start;
    UINT32                  duration;
    tBTA_DM_SEARCH_CBACK    *p_cback;
    tBTA_START_STOP_SCAN_CMPL_CBACK *p_start_scan_cback;
    tBTA_START_STOP_SCAN_CMPL_CBACK *p_stop_scan_cback;
    tBTA_START_STOP_ADV_CMPL_CBACK  *p_stop_adv_cback;
} tBTA_DM_API_BLE_OBSERVE;

#if (BLE_DATA_LEN_EXT_INCLUDED == TRUE)
typedef struct
{
    BT_HDR      hdr;
    BD_ADDR     remote_bda;
    UINT16      tx_data_length;
}tBTA_DM_API_BLE_SET_DATA_LEN;
#endif

#if BLE_BRCM_INCLUDED == TRUE

typedef struct
{
    BT_HDR                      hdr;
    BD_ADDR                     conn_bda;
    tBTA_DM_BLE_RSSI_ALERT_MASK alert_mask;
    UINT8                       low_threshold;
    UINT8                       range;
    UINT8                       hi_threshold;
    tBTA_DM_BLE_RSSI_CBACK      *p_cback;
} tBTA_DM_API_RSSI_MONITOR;


typedef struct
{
    BT_HDR                          hdr;
    tBTA_DM_BLE_SCAN_COND_OP        action;
    tBTA_DM_BLE_PF_COND_TYPE        cond_type;
    tBTA_DM_BLE_PF_FILT_INDEX       filt_index;
    tBTA_DM_BLE_PF_COND_PARAM       *p_cond_param;
    tBTA_DM_BLE_PF_CFG_CBACK      *p_filt_cfg_cback;
    tBTA_DM_BLE_REF_VALUE            ref_value;
}tBTA_DM_API_CFG_FILTER_COND;

typedef struct
{
    BT_HDR                          hdr;
    UINT8                           action;
    tBTA_DM_BLE_PF_STATUS_CBACK    *p_filt_status_cback;
    tBTA_DM_BLE_REF_VALUE            ref_value;
}tBTA_DM_API_ENABLE_SCAN_FILTER;
typedef struct
{
    BT_HDR                          hdr;
    UINT8                           action;
    tBTA_DM_BLE_PF_FILT_INDEX       filt_index;
    tBTA_DM_BLE_PF_FILT_PARAMS      filt_params;
    tBLE_BD_ADDR                    *p_target;
    tBTA_DM_BLE_PF_PARAM_CBACK      *p_filt_param_cback;
    tBTA_DM_BLE_REF_VALUE            ref_value;
}tBTA_DM_API_SCAN_FILTER_PARAM_SETUP;

typedef struct
{
    BT_HDR                  hdr;
    BOOLEAN                 enable;

}tBTA_DM_API_BLE_BRCM_FEATURE;

/* multi adv data structure */
typedef struct
{
    BT_HDR                      hdr;
    void                        *p_cback;
    void                        *p_ref;
    tBTA_BLE_ADV_PARAMS         *p_params;
}tBTA_DM_API_BLE_MULTI_ADV_ENB;

typedef struct
{
    BT_HDR                      hdr;
    UINT8                        inst_id;
    tBTA_BLE_ADV_PARAMS         *p_params;
}tBTA_DM_API_BLE_MULTI_ADV_PARAM;

typedef struct
{
    BT_HDR                  hdr;
    UINT8                   inst_id;
    BOOLEAN                 is_scan_rsp;
    tBTA_BLE_AD_MASK        data_mask;
    tBTA_BLE_ADV_DATA      *p_data;
}tBTA_DM_API_BLE_MULTI_ADV_DATA;

typedef struct
{
    BT_HDR                  hdr;
    UINT8                   inst_id;
}tBTA_DM_API_BLE_MULTI_ADV_DISABLE;

#endif /* BLE_BRCM_INCLUDED */
/* set adv parameter for BLE advertising */
typedef struct
{
    BT_HDR                  hdr;
    UINT16                  adv_int_min;
    UINT16                  adv_int_max;
    tBLE_BD_ADDR            *p_dir_bda;
}tBTA_DM_API_BLE_ADV_PARAMS;

/* set adv parameter for BLE advertising */
typedef struct {
    BT_HDR                  hdr;
    UINT16                  adv_int_min;
    UINT16                  adv_int_max;
    UINT8                   adv_type;
    tBLE_ADDR_TYPE          addr_type_own;
    tBTM_BLE_ADV_CHNL_MAP   channel_map;
    tBTM_BLE_AFP            adv_filter_policy;
    tBLE_BD_ADDR            *p_dir_bda;
    tBTA_START_ADV_CMPL_CBACK  *p_start_adv_cback;
} tBTA_DM_API_BLE_ADV_PARAMS_ALL;

typedef struct
{
    BT_HDR                  hdr;
    UINT32                  data_mask;
    tBTA_BLE_ADV_DATA       *p_adv_cfg;
    tBTA_SET_ADV_DATA_CMPL_CBACK    *p_adv_data_cback;
    BOOLEAN                 is_scan_rsp;
}tBTA_DM_API_SET_ADV_CONFIG;

/* raw scan response and raw advertising data use
   the same structure */
typedef struct {
    BT_HDR                  hdr;
    UINT8                   *p_raw_adv;
    UINT32                  raw_adv_len;
    tBTA_SET_ADV_DATA_CMPL_CBACK    *p_adv_data_cback;
    BOOLEAN                 is_scan_rsp;
} tBTA_DM_API_SET_ADV_CONFIG_RAW;

typedef struct
{
    BT_HDR                  hdr;
    UINT8                   batch_scan_full_max;
    UINT8                   batch_scan_trunc_max;
    UINT8                   batch_scan_notify_threshold;
    tBTA_BLE_SCAN_SETUP_CBACK *p_setup_cback;
    tBTA_BLE_SCAN_THRESHOLD_CBACK *p_thres_cback;
    tBTA_BLE_SCAN_REP_CBACK *p_read_rep_cback;
    tBTA_DM_BLE_REF_VALUE    ref_value;
} tBTA_DM_API_SET_STORAGE_CONFIG;

typedef struct
{
    BT_HDR                  hdr;
    tBTA_BLE_BATCH_SCAN_MODE  scan_mode;
    UINT32                  scan_int;
    UINT32                  scan_window;
    tBTA_BLE_DISCARD_RULE   discard_rule;
    tBLE_ADDR_TYPE          addr_type;
    tBTA_DM_BLE_REF_VALUE   ref_value;
} tBTA_DM_API_ENABLE_SCAN;

typedef struct
{
    BT_HDR                  hdr;
    tBTA_DM_BLE_REF_VALUE    ref_value;
} tBTA_DM_API_DISABLE_SCAN;

typedef struct
{
    BT_HDR                  hdr;
    tBTA_BLE_BATCH_SCAN_MODE scan_type;
    tBTA_DM_BLE_REF_VALUE    ref_value;
} tBTA_DM_API_READ_SCAN_REPORTS;

typedef struct
{
    BT_HDR                  hdr;
    tBTA_DM_BLE_REF_VALUE ref_value;
    tBTA_BLE_TRACK_ADV_CBACK *p_track_adv_cback;
} tBTA_DM_API_TRACK_ADVERTISER;

typedef struct
{
    BT_HDR                  hdr;
    tBTA_BLE_ENERGY_INFO_CBACK *p_energy_info_cback;
} tBTA_DM_API_ENERGY_INFO;

typedef struct
{
    BT_HDR                  hdr;
    UINT16                  handle;
    tBTA_BLE_PHY_INFO_CBACK *p_read_phy_cback;
} tBTA_DM_API_READ_CONN_PHY_INFO;
typedef struct
{
    BT_HDR                  hdr;
    UINT16                  handle;
    UINT8                   tx_phy;
    UINT8                   rx_phy;
    UINT16                  phy_options;
    tBTA_BLE_PHY_INFO_CBACK *p_set_phy_cback;
} tBTA_DM_API_SET_CONN_PHY_INFO;
typedef struct
{
    BT_HDR                  hdr;
    BD_ADDR bd_addr;
    tBLE_ADDR_TYPE addr_type;
    BOOLEAN is_direct;
} tBTA_DM_API_BLE_CREATE_CONN;
#endif

#if ( BTM_EIR_SERVER_INCLUDED == TRUE )&&( BTA_EIR_CANNED_UUID_LIST != TRUE )&&(BTA_EIR_SERVER_NUM_CUSTOM_UUID > 0)
/* data type for BTA_DM_API_UPDATE_EIR_UUID_EVT */
typedef struct
{
    BT_HDR          hdr;
    BOOLEAN         is_add;
    tBT_UUID        uuid;
}tBTA_DM_API_UPDATE_EIR_UUID;
#endif

#if (BTM_EIR_SERVER_INCLUDED == TRUE)
/* data type for BTA_DM_API_SET_EIR_CONFIG_EVT */
typedef struct
{
    BT_HDR              hdr;
    tBTA_DM_EIR_CONF    *p_eir_cfg;
}tBTA_DM_API_SET_EIR_CONFIG;
#endif

/* data type for BTA_DM_API_REMOVE_ACL_EVT */
typedef struct
{
    BT_HDR      hdr;
    BD_ADDR     bd_addr;
    BOOLEAN     remove_dev;
    tBTA_TRANSPORT transport;

}tBTA_DM_API_REMOVE_ACL;

/* data type for BTA_DM_API_REMOVE_ALL_ACL_EVT */
typedef struct {
    BT_HDR      hdr;
    tBTA_DM_LINK_TYPE link_type;

} tBTA_DM_API_REMOVE_ALL_ACL;

/* data type for BTA_DM_API_COEX_OP_EVT */
typedef struct
{
    BT_HDR                  hdr;
    UINT8                   coex_op;
    UINT8                   ctr_id;
    tBTA_DM_COEX_OP_PARAMS  coex_op_params;
}tBTA_DM_API_COEX_OP;

/* data type for BTA_DM_API_REG_COEX_CBACK_EVT */
typedef struct
{
    BT_HDR              hdr;
    tBTA_DM_COEX_CBACK  *p_coex_cback;
} tBTA_DM_API_REG_COEX_CBACK;

typedef struct
{
    BT_HDR      hdr;
    BD_ADDR     bd_addr;
    UINT16      min_int;
    UINT16      max_int;
    UINT16      latency;
    UINT16      timeout;
}tBTA_DM_API_UPDATE_CONN_PARAM;

typedef struct
{
    BT_HDR      hdr;
    BD_ADDR     bd_addr;
    tBTM_STATUS res;
    UINT32      passkey;
}tBTA_DM_API_PASSKEY_REQ_REPLY;

#if (defined(BTA_MPS_INCLUDED) && BTA_MPS_INCLUDED == TRUE)
/* BTA_DmSetMpsRecord internal message parameters */
typedef struct
{
    BT_HDR                  hdr;
    BOOLEAN                 expose_record; /* TRUE to create SDP, False to delete */
    tBTA_DM_MPS_PARAMS      params;
    tBTA_DM_SEARCH_CBACK    *p_cback;
}tBTA_DM_API_SET_MPS_RECORD;

/* BTA_DmMpsDiscover internal message parameters */
typedef struct
{
    BT_HDR                  hdr;
    BD_ADDR                 bd_addr;
    tBTA_DM_SEARCH_CBACK    *p_cback;
}tBTA_DM_API_MPS_DISCOVER;

#endif  /* BTA_MPS_INCLUDED */

/* union of all data types */
typedef union
{
    /* GKI event buffer header */
    BT_HDR              hdr;
    tBTA_DM_API_ENABLE  enable;

    tBTA_DM_API_SET_NAME set_name;

    tBTA_DM_API_SET_VISIBILITY set_visibility;

    tBTA_DM_API_SET_SCAN_CONFIG scan_config;

    tBTA_DM_API_SET_AFH_CHANNELS_EVT set_afhchannels;

    tBTA_DM_API_VENDOR_SPECIFIC_COMMAND vendor_command;

    tBTA_DM_API_ADD_DEVICE  add_dev;

    tBTA_DM_API_REMOVE_DEVICE remove_dev;

    tBTA_DM_API_SEARCH search;

    tBTA_DM_API_DISCOVER discover;

    tBTA_DM_API_BOND bond;

    tBTA_DM_API_BOND_CANCEL bond_cancel;

    tBTA_DM_API_PIN_REPLY pin_reply;
    tBTA_DM_API_LINK_POLICY link_policy;

    tBTA_DM_API_LOC_OOB     loc_oob;
    tBTA_DM_API_CONFIRM     confirm;
    tBTA_DM_API_PASKY_CANCEL passkey_cancel;
    tBTA_DM_CI_IO_REQ       ci_io_req;
    tBTA_DM_CI_RMT_OOB      ci_rmt_oob;
#if (BTM_BR_SC_INCLUDED == TRUE)
    tBTA_DM_CI_RMT_OOB_EXT  ci_rmt_oob_ext;
#endif

    tBTA_DM_API_AUTH_REPLY auth_reply;

    tBTA_DM_REM_NAME rem_name;

    tBTA_DM_DISC_RESULT disc_result;

    tBTA_DM_INQUIRY_CMPL inq_cmpl;

    tBTA_DM_SDP_RESULT sdp_event;

    tBTA_API_DM_SIG_STRENGTH sig_strength;

    tBTA_API_DM_TX_INQPWR   tx_inq_pwr;

    tBTA_DM_ACL_CHANGE  acl_change;

    tBTA_DM_PM_BTM_STATUS pm_status;

    tBTA_DM_PM_TIMER pm_timer;

    tBTA_DM_API_DI_DISC     di_disc;

    tBTA_DM_API_EXECUTE_CBACK exec_cback;

    tBTA_DM_API_SET_ENCRYPTION     set_encryption;

    tBTA_DM_API_READ_REMOTE_NAME    read_remote_name;

    tBTA_DM_API_SET_LOCAL_ADDR  set_local_addr;

#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
    tBTA_DM_API_SWITCH_STACK switch_stack;
#endif

#if BLE_INCLUDED == TRUE
    tBTA_DM_API_BLE_ENABLE              ble_enable;
    tBTA_DM_API_ADD_BLEKEY              add_ble_key;
    tBTA_DM_API_ADD_BLE_DEVICE          add_ble_device;
    tBTA_DM_API_PASSKEY_REPLY           ble_passkey_reply;
    tBTA_DM_API_BLE_SEC_GRANT           ble_sec_grant;
    tBTA_DM_API_BLE_SET_BG_CONN_TYPE    ble_set_bd_conn_type;
    tBTA_DM_API_BLE_CONN_PARAMS         ble_set_conn_params;
    tBTA_DM_API_BLE_SCAN_PARAMS         ble_set_scan_params;
    tBTA_DM_API_BLE_CONN_SCAN_PARAMS    ble_set_conn_scan_params;
    tBTA_DM_API_ENABLE_PRIVACY          ble_remote_privacy;
    tBTA_DM_API_LOCAL_PRIVACY           ble_local_privacy;
    tBTA_DM_API_BLE_ADV_PARAMS          ble_set_adv_params;
    tBTA_DM_API_BLE_ADV_PARAMS_ALL      ble_set_adv_params_all;
    tBTA_DM_API_SET_ADV_CONFIG          ble_set_adv_data;
    tBTA_DM_API_SET_ADV_CONFIG_RAW      ble_set_adv_data_raw;
    tBTA_DM_API_BLE_OBSERVE             ble_observe;
    tBTA_DM_API_UPDATE_CONN_PARAM       ble_update_conn_params;
#if (BLE_DATA_LEN_EXT_INCLUDED == TRUE)
    tBTA_DM_API_BLE_SET_DATA_LEN        ble_set_data_len;
#endif
#if BLE_BRCM_INCLUDED == TRUE
    tBTA_DM_API_RSSI_MONITOR            ble_rssi_monitor;
    //tBTA_DM_API_ENABLE_ADV_FILTER       ble_enable_adv_filter;
    //tBTA_DM_API_CFG_FILTER_COND         ble_cfg_filter_cond;
    tBTA_DM_API_BLE_BRCM_FEATURE        ble_brcm_feature;
    tBTA_DM_API_BLE_MULTI_ADV_ENB       ble_multi_adv_enb;
    tBTA_DM_API_BLE_MULTI_ADV_PARAM     ble_multi_adv_param;
    tBTA_DM_API_BLE_MULTI_ADV_DATA      ble_multi_adv_data;
    tBTA_DM_API_BLE_MULTI_ADV_DISABLE   ble_multi_adv_disable;
#endif
    tBTA_DM_API_SET_STORAGE_CONFIG      ble_set_storage;
    tBTA_DM_API_ENABLE_SCAN             ble_enable_scan;
    tBTA_DM_API_READ_SCAN_REPORTS       ble_read_reports;
    tBTA_DM_API_DISABLE_SCAN            ble_disable_scan;
    tBTA_DM_API_TRACK_ADVERTISER        ble_track_advert;
    tBTA_DM_API_ENERGY_INFO             ble_energy_info;
    tBTA_DM_API_READ_CONN_PHY_INFO      ble_read_conn_phy;
    tBTA_DM_API_SET_CONN_PHY_INFO       ble_set_conn_phy;
    tBTA_DM_API_BLE_CREATE_CONN         ble_create_conn_parms;
#if BLE_SCAN_FILTER == TRUE
    tBTA_DM_API_SCAN_FILTER_PARAM_SETUP ble_scan_filt_param_setup;
    tBTA_DM_API_CFG_FILTER_COND         ble_cfg_filter_cond;
    tBTA_DM_API_ENABLE_SCAN_FILTER      ble_enable_scan_filt;
#endif
#endif

#if ( BTM_EIR_SERVER_INCLUDED == TRUE )&&( BTA_EIR_CANNED_UUID_LIST != TRUE )&&(BTA_EIR_SERVER_NUM_CUSTOM_UUID > 0)
    tBTA_DM_API_UPDATE_EIR_UUID     update_eir_uuid;
#endif
#if (BTM_EIR_SERVER_INCLUDED == TRUE)
    tBTA_DM_API_SET_EIR_CONFIG          set_eir_cfg;
#endif
    tBTA_DM_API_REMOVE_ACL              remove_acl;
#if (defined BTM_TBFC_INCLUDED) && (BTM_TBFC_INCLUDED == TRUE)
    tBTA_DM_API_SET_TBFC_MODE           set_tbfc_mode;
#endif
    tBTA_DM_API_COEX_OP                 coex_op;
    tBTA_DM_API_REG_COEX_CBACK          reg_coex_cback;
    tBTA_DM_API_PASSKEY_REQ_REPLY       passkey_req_reply;

#if (defined(BTA_MPS_INCLUDED) && BTA_MPS_INCLUDED == TRUE)
    tBTA_DM_API_SET_MPS_RECORD          set_mps_sdp;
    tBTA_DM_API_MPS_DISCOVER            mps_disc;
#endif

    tBTA_DM_API_REMOVE_ALL_ACL          remove_all_acl;
} tBTA_DM_MSG;


#define BTA_DM_NUM_PEER_DEVICE 7

#define BTA_DM_NOT_CONNECTED  0
#define BTA_DM_CONNECTED      1
#define BTA_DM_UNPAIRING      2
typedef UINT8 tBTA_DM_CONN_STATE;


#define BTA_DM_DI_NONE          0x00       /* nothing special */
#define BTA_DM_DI_USE_SSR       0x10       /* set this bit if ssr is supported for this link */
#define BTA_DM_DI_AV_ACTIVE     0x20       /* set this bit if AV is active for this link */
#define BTA_DM_DI_SET_SNIFF     0x01       /* set this bit if call BTM_SetPowerMode(sniff) */
#define BTA_DM_DI_INT_SNIFF     0x02       /* set this bit if call BTM_SetPowerMode(sniff) & enter sniff mode */
#define BTA_DM_DI_ACP_SNIFF     0x04       /* set this bit if peer init sniff */
#if (BTM_TBFC_INCLUDED == TRUE)
#define BTA_DM_DI_SET_SUSPEND   0x08       /* set this bit if link has been suspened */
#endif
typedef UINT8 tBTA_DM_DEV_INFO;

/* set power mode request type */
#define BTA_DM_PM_RESTART       1
#define BTA_DM_PM_NEW_REQ       2
#define BTA_DM_PM_EXECUTE       3
typedef UINT8   tBTA_DM_PM_REQ;

#define BTA_DM_MAX_ENC_CBACK_NUM        4

typedef struct
{
    BT_HDR                  *p_hdr;
    tBTA_DM_ENCRYPT_CBACK   *p_encrypt_cback;
}tBTM_DM_ENC_REQ;

typedef struct
{
    BD_ADDR                     peer_bdaddr;
    UINT16                      link_policy;
    tBTA_DM_CONN_STATE          conn_state;
    tBTA_PREF_ROLES             pref_role;
    BOOLEAN                     in_use;
    tBTA_DM_DEV_INFO            info;
    BUFFER_Q                    enc_req_q;

#if (BTM_SSR_INCLUDED == TRUE)
    tBTM_PM_STATUS              prev_low;   /* previous low power mode used */
#endif
    tBTA_DM_PM_ACTION           pm_mode_attempted;
    tBTA_DM_PM_ACTION           pm_mode_failed;
    BOOLEAN                     remove_dev_pending;
#if BLE_INCLUDED == TRUE
    UINT16                      conn_handle;
    tBT_TRANSPORT               transport;
#endif
} tBTA_DM_PEER_DEVICE;

/* part of tBTA_DM_PEER_DEVICE */
typedef struct
{
    BD_ADDR                     peer_bdaddr;
    UINT16                      link_policy;
    tBTA_DM_CONN_STATE          conn_state;
    tBTA_PREF_ROLES             pref_role;
    BOOLEAN                     in_use;
    tBTA_DM_DEV_INFO            info;
} tBTA_DM_LPST_PEER_DEVICE;


/* structure to store list of
  active connections */
typedef struct
{
    tBTA_DM_PEER_DEVICE    peer_device[BTA_DM_NUM_PEER_DEVICE];
    UINT8                  count;
#if BLE_INCLUDED == TRUE
    UINT8                  le_count;
#endif
} tBTA_DM_ACTIVE_LINK;


typedef struct
{
    BD_ADDR                 peer_bdaddr;
    tBTA_SYS_ID             id;
    UINT8                   app_id;
    tBTA_SYS_CONN_STATUS    state;
    BOOLEAN                 new_request;

} tBTA_DM_SRVCS;

#ifndef BTA_DM_NUM_CONN_SRVS
#define BTA_DM_NUM_CONN_SRVS   10
#endif

typedef struct
{

    UINT8 count;
    tBTA_DM_SRVCS  conn_srvc[BTA_DM_NUM_CONN_SRVS];

}  tBTA_DM_CONNECTED_SRVCS;

typedef struct
{
#define BTA_DM_PM_SNIFF_TIMER_IDX   0
#define BTA_DM_PM_PARK_TIMER_IDX    1
#define BTA_DM_PM_SUSPEND_TIMER_IDX 2
#define BTA_DM_PM_MODE_TIMER_MAX    3
    TIMER_LIST_ENT          timer[BTA_DM_PM_MODE_TIMER_MAX];  /* keep three different timers for PARK, SNIFF and SUSPEND if TBFC is supporteds */
    UINT8                   srvc_id[BTA_DM_PM_MODE_TIMER_MAX];
    UINT8                   app_id[BTA_DM_PM_MODE_TIMER_MAX];
    UINT8                   pm_action[BTA_DM_PM_MODE_TIMER_MAX];
    UINT8                   active;     /* number of active timer */

    BD_ADDR                 peer_bdaddr;
    BOOLEAN                 in_use;
} tBTA_PM_TIMER;

extern tBTA_DM_CONNECTED_SRVCS bta_dm_conn_srvcs;

#define BTA_DM_NUM_PM_TIMER 3

/* DM control block */
typedef struct
{
    BOOLEAN                     is_bta_dm_active;
    tBTA_DM_ACTIVE_LINK         device_list;
    tBTA_DM_SEC_CBACK           *p_sec_cback;

#if ((defined BLE_INCLUDED) && (BLE_INCLUDED == TRUE))
    tBTA_BLE_SCAN_SETUP_CBACK   *p_setup_cback;
    tBTA_DM_BLE_PF_CFG_CBACK     *p_scan_filt_cfg_cback;
    tBTA_DM_BLE_PF_STATUS_CBACK  *p_scan_filt_status_cback;
    tBTA_DM_BLE_PF_PARAM_CBACK   *p_scan_filt_param_cback;
    tBTA_BLE_ENERGY_INFO_CBACK   *p_energy_info_cback;
#endif
    TIMER_LIST_ENT              signal_strength_timer;
    tBTA_SIG_STRENGTH_MASK      signal_strength_mask;
    UINT16                      signal_strength_period;
    BOOLEAN                     disabling;
    TIMER_LIST_ENT              disable_timer;
    UINT8                       pm_id;
    tBTA_PM_TIMER               pm_timer[BTA_DM_NUM_PM_TIMER];
    UINT32                      role_policy_mask;   /* the bits set indicates the modules that wants to remove role switch from the default link policy */
    UINT16                      cur_policy;         /* current default link policy */
    UINT16                      rs_event;           /* the event waiting for role switch */
    UINT8                       cur_av_count;       /* current AV connecions */
    BOOLEAN                     disable_pair_mode;          /* disable pair mode or not */
    BOOLEAN                     conn_paired_only;   /* allow connectable to paired device only or not */

    tBTA_DM_API_SEARCH          search_msg;
    UINT16                      page_scan_interval;
    UINT16                      page_scan_window;
    UINT16                      inquiry_scan_interval;
    UINT16                      inquiry_scan_window;

    /* Storage for pin code request parameters */
    BD_ADDR                     pin_bd_addr;
    DEV_CLASS                   pin_dev_class;
    tBTA_DM_SEC_EVT             pin_evt;
    UINT32          num_val;        /* the numeric value for comparison. If just_works, do not show this number to UI */
    BOOLEAN         just_works;     /* TRUE, if "Just Works" association model */
#if ( BTM_EIR_SERVER_INCLUDED == TRUE )&&( BTA_EIR_CANNED_UUID_LIST != TRUE )
    /* store UUID list for EIR */
    TIMER_LIST_ENT              app_ready_timer;
    UINT32                      eir_uuid[BTM_EIR_SERVICE_ARRAY_SIZE];
#if (BTA_EIR_SERVER_NUM_CUSTOM_UUID > 0)
    tBT_UUID                    custom_uuid[BTA_EIR_SERVER_NUM_CUSTOM_UUID];
#endif

#endif
#if (BTM_EIR_SERVER_INCLUDED == TRUE)
    UINT8                      *p_eir_dyn_brcm_aware;   /* point of dynamic BRCM Aware EIR data */
#endif

#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
    tBTM_SW_DIR                 switch_dir;
    tBTA_SYS_SWITCH_STATUS      switched_to_lite;
    tBTA_DM_SWITCH_CBACK        *p_switch_stack_cback;
    tBTA_SYS_IPC_EVT_CBACK      *p_bta_av_ipc_evt_cback;
    tBTA_SYS_NOTIFY_CBACK       *p_bta_av_notify_cback;
    tBTA_SYS_SYNC_REQ_CBACK     *p_bta_av_sync_req_cback;
#endif
#if (BTU_BTC_SNK_INCLUDED == TRUE)
    BOOLEAN                     avk_opened;         /* AVK connection is opened */
    tBTA_SYS_IPC_EVT_CBACK      *p_bta_avk_ipc_evt_cback;
    tBTA_SYS_SYNC_REQ_CBACK     *p_bta_avk_sync_req_cback;
#endif

    tBTA_DM_ENCRYPT_CBACK      *p_encrypt_cback;
    tBTA_DM_COEX_CBACK         *p_coex_cback;
    tBTA_SET_ADDR_CMPL_CBACK   *p_set_local_addr_cback;

#if (defined(BTA_MPS_INCLUDED) && BTA_MPS_INCLUDED == TRUE)
    tBTA_DM_SEARCH_CBACK        *p_mps_disc_cb;
    tSDP_DISCOVERY_DB           *p_mps_sdp_db;  /* pointer to the DI discovery database */
    UINT32                      mps_sdp_handle; /* SDP record handle */
#endif
} tBTA_DM_CB;

#ifndef BTA_DM_SDP_DB_SIZE
#define BTA_DM_SDP_DB_SIZE 750
#endif

/* DM search control block */
typedef struct
{

    tBTA_DM_SEARCH_CBACK * p_search_cback;
    tBTM_INQ_INFO        * p_btm_inq_info;
    tBTA_SERVICE_MASK      services;
    tBTA_SERVICE_MASK      services_to_search;
    tBTA_SERVICE_MASK      services_found;
    tSDP_DISCOVERY_DB    * p_sdp_db;
    UINT16                 state;
    BD_ADDR                peer_bdaddr;
    BOOLEAN                name_discover_done;
    BD_NAME                peer_name;
    TIMER_LIST_ENT         search_timer;
    UINT8                  service_index;
    tBTA_DM_MSG          * p_search_queue;   /* search or discover commands during search cancel stored here */
    BOOLEAN                wait_disc;
    BOOLEAN                sdp_results;
    BOOLEAN                sdp_search;
    BOOLEAN                cancel_pending; /* inquiry cancel is pending */
    tBTA_TRANSPORT          transport;

#if ((defined BLE_INCLUDED) && (BLE_INCLUDED == TRUE))
    tBTA_DM_SEARCH_CBACK * p_scan_cback;
#if ((defined BTA_GATT_INCLUDED) && (BTA_GATT_INCLUDED == TRUE))
    tBTA_GATTC_IF          client_if;
    UINT8                  num_uuid;
    tBT_UUID               *p_srvc_uuid;
    UINT8                  uuid_to_search;
    BOOLEAN                gatt_disc_active;
    UINT16                 conn_id;
    TIMER_LIST_ENT         gatt_close_timer; /* GATT channel close delay timer */
    BD_ADDR                pending_close_bda; /* pending GATT channel remote device address */
#endif
#endif

} tBTA_DM_SEARCH_CB;

/* DI control block */
typedef struct
{
    tSDP_DISCOVERY_DB    * p_di_db;     /* pointer to the DI discovery database */
    UINT8               di_num;         /* total local DI record number */
    UINT32              di_handle[BTA_DI_NUM_MAX];  /* local DI record handle, the first one is primary record */
}tBTA_DM_DI_CB;

/* DM search state */
enum
{

    BTA_DM_SEARCH_IDLE,
    BTA_DM_SEARCH_ACTIVE,
    BTA_DM_SEARCH_CANCELLING,
    BTA_DM_DISCOVER_ACTIVE
};


typedef struct
{
    DEV_CLASS      dev_class;          /* local device class */
    UINT16         policy_settings;    /* link policy setting hold, sniff, park, MS switch */
    UINT16         page_timeout;       /* timeout for page in slots */
    UINT16         link_timeout;       /* link supervision timeout in slots */
    BOOLEAN        avoid_scatter;      /* TRUE to avoid scatternet when av is streaming (be the master) */

} tBTA_DM_CFG;

extern const UINT32 bta_service_id_to_btm_srv_id_lkup_tbl[];

extern const tBTA_DM_CFG bta_dm_cfg;



typedef struct
{
    UINT8   id;
    UINT8   app_id;
    UINT8   cfg;

} tBTA_DM_RM ;

extern tBTA_DM_CFG *p_bta_dm_cfg;
extern tBTA_DM_RM *p_bta_dm_rm_cfg;

typedef struct
{

  UINT8  id;
  UINT8  app_id;
  UINT8  spec_idx;  /* index of spec table to use */

} tBTA_DM_PM_CFG;


typedef struct
{

  tBTA_DM_PM_ACTION power_mode;
  UINT16            timeout;

} tBTA_DM_PM_ACTN;

typedef struct
{

  UINT8  allow_mask;         /* mask of sniff/hold/park modes to allow */
#if (BTM_SSR_INCLUDED == TRUE)
  UINT8  ssr;                /* set SSR on conn open/unpark */
#endif
  tBTA_DM_PM_ACTN actn_tbl [BTA_DM_PM_NUM_EVTS][2];

} tBTA_DM_PM_SPEC;

typedef struct
{
    UINT16      max_lat;
    UINT16      min_rmt_to;
    UINT16      min_loc_to;
} tBTA_DM_SSR_SPEC;

#if (defined BTM_TBFC_INCLUDED) && (BTM_TBFC_INCLUDED == TRUE)
typedef struct
{
    UINT8   id;
    UINT8   app_id;
    BOOLEAN use_tbfc;
}tBTA_DM_PM_TBFC_CFG;

extern tBTA_DM_PM_TBFC_CFG *p_bta_dm_pm_tbfc_cfg;

#endif
extern tBTA_DM_PM_CFG *p_bta_dm_pm_cfg;
extern tBTA_DM_PM_SPEC *p_bta_dm_pm_spec;
extern tBTM_PM_PWR_MD *p_bta_dm_pm_md;
#if (BTM_SSR_INCLUDED == TRUE)
extern tBTA_DM_SSR_SPEC *p_bta_dm_ssr_spec;
#endif

#if ( BTM_EIR_SERVER_INCLUDED == TRUE )
/* update dynamic BRCM Aware EIR data */
extern BOOLEAN bta_dm_eir_add_brcm_spec( UINT8 type, UINT8 length, const UINT8 *p_data);
extern BOOLEAN bta_dm_eir_remove_brcm_spec( UINT8 type, UINT8 length, const UINT8 *p_data);

extern const tBTA_DM_EIR_CONF bta_dm_eir_cfg;
extern tBTA_DM_EIR_CONF *p_bta_dm_eir_cfg;
#endif

/* DM control block */
#if BTA_DYNAMIC_MEMORY == FALSE
extern tBTA_DM_CB  bta_dm_cb;
#else
extern tBTA_DM_CB *bta_dm_cb_ptr;
#define bta_dm_cb (*bta_dm_cb_ptr)
#endif

/* DM search control block */
#if BTA_DYNAMIC_MEMORY == FALSE
extern tBTA_DM_SEARCH_CB  bta_dm_search_cb;
#else
extern tBTA_DM_SEARCH_CB *bta_dm_search_cb_ptr;
#define bta_dm_search_cb (*bta_dm_search_cb_ptr)
#endif

/* DI control block */
#if BTA_DYNAMIC_MEMORY == FALSE
extern tBTA_DM_DI_CB  bta_dm_di_cb;
#else
extern tBTA_DM_DI_CB *bta_dm_di_cb_ptr;
#define bta_dm_di_cb (*bta_dm_di_cb_ptr)
#endif

extern BOOLEAN bta_dm_sm_execute(BT_HDR *p_msg);
extern void bta_dm_sm_disable( void );
extern BOOLEAN bta_dm_search_sm_execute(BT_HDR *p_msg);
extern void bta_dm_search_sm_disable( void );


extern void bta_dm_enable (tBTA_DM_MSG *p_data);
extern void bta_dm_disable (tBTA_DM_MSG *p_data);
extern void bta_dm_set_dev_name (tBTA_DM_MSG *p_data);
extern void bta_dm_set_dev_name_le (tBTA_DM_MSG *p_data);
extern void bta_dm_set_visibility (tBTA_DM_MSG *p_data);
extern void bta_dm_set_scan_config (tBTA_DM_MSG *p_data);
extern void bta_dm_set_afhchannels (tBTA_DM_MSG *p_data);
extern void bta_dm_reg_wifichannotif(tBTA_DM_MSG *p_data);
extern void bta_dm_vendor_spec_command(tBTA_DM_MSG *p_data);
extern void bta_dm_bond (tBTA_DM_MSG *p_data);
extern void bta_dm_bond_cancel (tBTA_DM_MSG *p_data);
extern void bta_dm_pin_reply (tBTA_DM_MSG *p_data);
extern void bta_dm_link_policy (tBTA_DM_MSG *p_data);
extern void bta_dm_auth_reply (tBTA_DM_MSG *p_data);
extern void bta_dm_signal_strength(tBTA_DM_MSG *p_data);
extern void bta_dm_tx_inqpower(tBTA_DM_MSG *p_data);
extern void bta_dm_acl_change(tBTA_DM_MSG *p_data);
extern void bta_dm_add_device (tBTA_DM_MSG *p_data);
extern void bta_dm_remove_device (tBTA_DM_MSG *p_data);
extern void bta_dm_close_acl(tBTA_DM_MSG *p_data);

#if (defined BTM_TBFC_INCLUDED) && (BTM_TBFC_INCLUDED == TRUE)
extern void bta_dm_set_tbfc_scan_mode (tBTA_DM_MSG *p_data);
#endif

extern void bta_dm_pm_btm_status(tBTA_DM_MSG *p_data);
extern void bta_dm_pm_timer(tBTA_DM_MSG *p_data);
extern void bta_dm_add_ampkey (tBTA_DM_MSG *p_data);

#if BLE_INCLUDED == TRUE
extern void bta_dm_ble_enable(tBTA_DM_MSG *p_data);
extern void bta_dm_add_blekey (tBTA_DM_MSG *p_data);
extern void bta_dm_add_ble_device (tBTA_DM_MSG *p_data);
extern void bta_dm_ble_passkey_reply (tBTA_DM_MSG *p_data);
#if SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
extern void bta_dm_ble_confirm_reply (tBTA_DM_MSG *p_data);
#endif
extern void bta_dm_security_grant (tBTA_DM_MSG *p_data);
extern void bta_dm_ble_set_bg_conn_type (tBTA_DM_MSG *p_data);
extern void bta_dm_ble_set_conn_params (tBTA_DM_MSG *p_data);
extern void bta_dm_ble_set_scan_params (tBTA_DM_MSG *p_data);
extern void bta_dm_ble_set_conn_scan_params (tBTA_DM_MSG *p_data);
extern void bta_dm_close_gatt_conn(tBTA_DM_MSG *p_data);
extern void bta_dm_ble_update_conn_params (tBTA_DM_MSG *p_data);


#if BLE_BRCM_INCLUDED == TRUE && BTM_BLE_PRIVACY_SPT == TRUE
extern void bta_dm_ble_config_local_privacy (tBTA_DM_MSG *p_data);
#endif
extern void bta_dm_ble_set_adv_params (tBTA_DM_MSG *p_data);
extern void bta_dm_ble_observe (tBTA_DM_MSG *p_data);
extern void bta_dm_ble_set_adv_params_all(tBTA_DM_MSG *p_data);
extern void bta_dm_ble_set_adv_config (tBTA_DM_MSG *p_data);
extern void bta_dm_ble_set_adv_config_raw (tBTA_DM_MSG *p_data);
extern void bta_dm_ble_broadcast (tBTA_DM_MSG *p_data);

#if BLE_BRCM_INCLUDED == TRUE
extern void bta_dm_ble_rssi_monitor_link (tBTA_DM_MSG *p_data);
extern void bta_dm_ble_rssi_monitor_adv (tBTA_DM_MSG *p_data);
extern void bta_dm_enable_adv_filter (tBTA_DM_MSG *p_data);
extern void bta_dm_cfg_filter_cond (tBTA_DM_MSG *p_data);
extern void bta_dm_ble_scatternet_enable(tBTA_DM_MSG *p_data);
extern void bta_dm_ble_brcm_feat_enable(tBTA_DM_MSG *p_data);
extern void btm_dm_ble_multi_adv_disable(tBTA_DM_MSG *p_data);
extern void bta_dm_ble_multi_adv_data(tBTA_DM_MSG *p_data);
extern void bta_dm_ble_multi_adv_param_upd(tBTA_DM_MSG *p_data);
extern void bta_dm_ble_multi_adv_enb(tBTA_DM_MSG *p_data);
#endif
extern void bta_dm_ble_setup_storage(tBTA_DM_MSG *p_data);
extern void bta_dm_ble_enable_batch_scan(tBTA_DM_MSG * p_data);
extern void bta_dm_ble_disable_batch_scan(tBTA_DM_MSG * p_data);
extern void bta_dm_ble_read_scan_reports(tBTA_DM_MSG * p_data);
extern void bta_dm_ble_track_advertiser(tBTA_DM_MSG * p_data);
extern void bta_dm_ble_get_energy_info(tBTA_DM_MSG *p_data);
extern void bta_dm_ble_read_conn_phy_info(tBTA_DM_MSG *p_data);
extern void bta_dm_ble_set_conn_phy(tBTA_DM_MSG *p_data);
extern void bta_dm_ble_create_conn(tBTA_DM_MSG *p_data);
#if BLE_SCAN_FILTER == TRUE
extern void bta_dm_cfg_filter_cond (tBTA_DM_MSG *p_data);
extern void bta_dm_scan_filter_param_setup (tBTA_DM_MSG *p_data);
extern void bta_dm_enable_scan_filter(tBTA_DM_MSG *p_data);
#endif
extern void bta_dm_ctrl_features_rd_cmpl_cback(tBTM_STATUS result);

#endif
#if BLE_DATA_LEN_EXT_INCLUDED == TRUE
extern void bta_dm_ble_set_data_len(tBTA_DM_MSG *p_data);
#endif
extern void bta_dm_set_encryption(tBTA_DM_MSG *p_data);
extern void bta_dm_read_remote_name(tBTA_DM_MSG *p_data);
extern void bta_dm_set_local_device_addr(tBTA_DM_MSG *p_data);
extern void bta_dm_confirm(tBTA_DM_MSG *p_data);
extern void bta_dm_passkey_cancel(tBTA_DM_MSG *p_data);
#if (BTM_OOB_INCLUDED == TRUE)
extern void bta_dm_loc_oob(tBTA_DM_MSG *p_data);
extern void bta_dm_ci_io_req_act(tBTA_DM_MSG *p_data);
extern void bta_dm_ci_rmt_oob_act(tBTA_DM_MSG *p_data);
#if (BTM_BR_SC_INCLUDED == TRUE)
extern void bta_dm_ci_rmt_oob_ext_act(tBTA_DM_MSG *p_data);
#endif /* BTM_BR_SC_INCLUDED */
#endif /* BTM_OOB_INCLUDED */

extern void bta_dm_init_pm(void);
extern void bta_dm_disable_pm(void);

extern UINT8 bta_dm_get_av_count(void);
extern void bta_dm_search_start (tBTA_DM_MSG *p_data);
extern void bta_dm_search_cancel (tBTA_DM_MSG *p_data);
extern void bta_dm_discover (tBTA_DM_MSG *p_data);
extern void bta_dm_di_disc (tBTA_DM_MSG *p_data);
extern void bta_dm_inq_cmpl (tBTA_DM_MSG *p_data);
extern void bta_dm_rmt_name (tBTA_DM_MSG *p_data);
extern void bta_dm_sdp_result (tBTA_DM_MSG *p_data);
extern void bta_dm_search_cmpl (tBTA_DM_MSG *p_data);
extern void bta_dm_free_sdp_db (tBTA_DM_MSG *p_data);
extern void bta_dm_disc_result (tBTA_DM_MSG *p_data);
extern void bta_dm_search_result (tBTA_DM_MSG *p_data);
extern void bta_dm_discovery_cmpl (tBTA_DM_MSG *p_data);
extern void bta_dm_queue_search (tBTA_DM_MSG *p_data);
extern void bta_dm_queue_disc (tBTA_DM_MSG *p_data);
extern void bta_dm_search_clear_queue (tBTA_DM_MSG *p_data);
extern void bta_dm_search_cancel_cmpl (tBTA_DM_MSG *p_data);
extern void bta_dm_search_cancel_notify (tBTA_DM_MSG *p_data);
extern void bta_dm_search_cancel_transac_cmpl(tBTA_DM_MSG *p_data);
extern void bta_dm_disc_rmt_name (tBTA_DM_MSG *p_data);
extern tBTA_DM_PEER_DEVICE * bta_dm_find_peer_device(BD_ADDR peer_addr);

extern void bta_dm_pm_active(BD_ADDR peer_addr);

#if ( BTM_EIR_SERVER_INCLUDED == TRUE )
void bta_dm_eir_update_uuid(UINT16 uuid16, BOOLEAN adding);
#else
#define bta_dm_eir_update_uuid(x, y)
#endif

#if (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE)
/*************************************************************************************/
/* Function called to switch stack when Application invokes BTA_DmSwitchStack() API  */
/*************************************************************************************/
extern void bta_dm_switch_stack (tBTA_DM_MSG *p_data);

/*************************************************************************************/
/* Function called by BTM (as a callback) to handle IPC events. This DM function     */
/* then calls the registered bta layer (bta_av) callback to inform of IPC events     */
/*************************************************************************************/
extern void bta_dm_hdl_ipc_evt(tBTM_STATUS status, BT_HDR *p_data);

extern void bta_dm_ipc_evt_hdlr(tBTA_DM_MSG *p_data);


/*************************************************************************************/
/* Function called by bta_sys when bta subsystems register a callback with DM.       */
/* The callback is invoked by DM when a IPC message received for the bta subsystem   */
/*************************************************************************************/
extern void bta_dm_register_ipc_cback(UINT8 subsys_id, tBTA_SYS_IPC_EVT_CBACK *p_cback);

/*************************************************************************************/
/* Function called by bta_sys when bta subsystems need to inform DM of the sync      */
/* result.                                                                           */
/* NOTE that the sync request would have been originated by DM and so it is being    */
/* informed of the result. DM initiates the bta sync process when App originates the */
/* bb->mm switch stack request.                                                      */
/*************************************************************************************/
extern void bta_dm_proc_sync_result(UINT8 subsys_id, tBTA_SYS_SYNC_RESULT *p_data);

/*************************************************************************************/
/* Function called by bta_sys when bta subsystems register 'notify' callbacks with   */
/* DM.                                                                               */
/*************************************************************************************/
extern void bta_dm_register_notify_cback(UINT8 subsys_id, tBTA_SYS_NOTIFY_CBACK *p_cback);

/*************************************************************************************/
/* Function called by bta_sys when bta subsystems need to ask DM whether switched    */
/* to Lite stack.                                                                    */
/* Returns TRUE if switched to Lite stack, else FALSE                                */
/*************************************************************************************/
extern BOOLEAN bta_dm_is_switched_to_lite(void);

/*************************************************************************************/
/* Function called by bta_sys when bta subsystems register a callback with DM.       */
/* The callback is invoked by DM when it wants to send sync request message to the   */
/* subsystem.                                                                        */
/*************************************************************************************/
extern void bta_dm_register_sync_cback(UINT8 subsys_id, tBTA_SYS_SYNC_REQ_CBACK *p_cback);

#endif /* (BTU_DUAL_STACK_MM_INCLUDED == TRUE) || (BTU_DUAL_STACK_BTC_INCLUDED == TRUE) */

#if ( BTM_EIR_SERVER_INCLUDED == TRUE )&&( BTA_EIR_CANNED_UUID_LIST != TRUE )&&(BTA_EIR_SERVER_NUM_CUSTOM_UUID > 0)
extern void bta_dm_update_eir_uuid (tBTA_DM_MSG *p_data);
#endif
#if (BTM_EIR_SERVER_INCLUDED == TRUE)
extern void bta_dm_set_eir_config (tBTA_DM_MSG *p_data);
#endif
extern void bta_dm_enable_test_mode(tBTA_DM_MSG *p_data);
extern void bta_dm_disable_test_mode(tBTA_DM_MSG *p_data);
extern void bta_dm_execute_callback(tBTA_DM_MSG *p_data);
extern void bta_dm_coex_op(tBTA_DM_MSG *p_data);
extern void bta_dm_reg_coex_cback(tBTA_DM_MSG *p_data);
extern void bta_dm_passkey_req_reply(tBTA_DM_MSG *p_data);

#if (defined(BTA_MPS_INCLUDED) && BTA_MPS_INCLUDED == TRUE)
extern void bta_dm_mps_sdp_register (tBTA_DM_MSG *p_data);
extern void bta_dm_mps_discover(tBTA_DM_MSG *p_data);
#endif
#if (BRCM_LPST_INCLUDED == TRUE)
#define BTA_DM_LPST_CB_DEV            0x01    // for tBTA_DM_PEER_DEVICE

extern void bta_dm_lpst_device_sync_request(BOOLEAN snapshot_sync);
extern void bta_dm_lpst_sync_handler(UINT8 sync_code, UINT8 *p, UINT16 cb_len);
extern BOOLEAN bta_dm_lpst_evt_cback (UINT8 event, tBTM_STATUS status, tBTM_LPST_INT_EVT_DATA *p_data);
extern void btm_dm_lpst_init(void);
extern BOOLEAN bta_lpst_send_control_blocks (BD_ADDR bd_addr);
extern void bta_lpst_process_control_blocks (UINT8 *p, UINT16 cb_len);

#endif


extern void bta_dm_read_rmt_rssi(BD_ADDR rm_addr, tBT_TRANSPORT transport);
extern void bta_dm_read_rm_tx_power(BD_ADDR rm_addr, tBT_TRANSPORT transport);

extern void bta_dm_remove_all_acl(tBTA_DM_MSG *p_data);
#endif /* BTA_DM_INT_H */



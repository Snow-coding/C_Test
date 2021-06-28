#ifdef __cplusplus
extern "C" {
#endif

#include "opple_sigmesh_application.h"
#include "string.h"

#define OPPLE_MEMSET memset
#define OPPLE_MEMCPY memcpy

#ifdef __cplusplus
extern "C" {
#endif

extern uint16_t Need_prov_dev_number;
extern int32_t app_dev_opple_put_char_switch_state_handler(uint16_t unicastAddr, uint8_t bool_stat);
extern int32_t app_dev_opple_put_char_brightness_state_handler(uint16_t unicastAddr, uint32_t bri);
extern int32_t app_dev_opple_put_char_cct_state_handler(uint16_t unicastAddr, uint32_t cct_val);
extern int opple_dummy_send_scan_result_to_cloud(uint8_t *buffer, uint16_t size);
extern int app_dev_profile_put_cloud_confirm_random(char *sn, char* cloudconfirmation, char *cloudrandom);
static opple_prov_ret_sync_callback custom_prov_ret_sync_callback = NULL;
static opple_prov_version_sync_callback custom_prov_version_sync_callback = NULL;
uint16_t dnlink1_calc_gpno(uint16_t devAddr, uint8_t objId)
{
    return 0;
}

/*******************************************************************************************************
                                    SIG MESH MESSAHE - PACK/UNPACK
********************************************************************************************************/
/* Sigmesh messahe head packing */
static int sig_cmd_head_packing(uint16_t dst, SIG_MESH_CMD_T *sig_pack)
{
    if (sig_pack == NULL) return -1;

    sig_pack->nk_idx = 0;
    sig_pack->ak_idx = 0;
    sig_pack->retry_cnt = 2;
    sig_pack->rsp_max = 0;
    sig_pack->adr_dst = dst;

    return 0;
}

/* Sigmesh gengeric onoff */
int sigmesh_generic_onoff_set_pack(uint16_t addr, const SIG_GENERIC_ONOFF_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_GENERIC_ONOFF_SET_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_GENERIC_ONOFF_SET_T));
    ((SIG_GENERIC_ONOFF_SET_T *)(sig_out->payload))->opcode = G_ONOFF_SET;
    *total_len = sizeof(SIG_GENERIC_ONOFF_SET_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_generic_onoff_status_unpack(const uint8_t *in, uint8_t in_len, uint16_t *addr, SIG_GENERIC_ONOFF_STATUS_T *out)
{
    if (in == NULL || out == NULL || addr == NULL || in_len <= sizeof(SIG_MESH_RSP_T) + sizeof(uint16_t)) return -1;

    SIG_MESH_RSP_T *sig_in   = (SIG_MESH_RSP_T *)in;
    uint8_t         data_len = in_len - sizeof(SIG_MESH_RSP_T);

     *addr = sig_in->src;
    if(data_len < sizeof(SIG_GENERIC_ONOFF_STATUS_T)){
        OPPLE_MEMCPY((void *)out, (void *)(sig_in->data),data_len);

        out->target_onoff = out->present_onoff;
        out->remain_time  = 0;
    } else
        OPPLE_MEMCPY((void *)out, (void *)(sig_in->data), sizeof(SIG_GENERIC_ONOFF_STATUS_T));

    return 0;
}

/* Sigmesh light - lightness */
int sigmesh_lighting_lightness_set_pack(uint16_t addr, const SIG_LIGHT_LIGHTNESS_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_LIGHT_LIGHTNESS_SET_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_LIGHT_LIGHTNESS_SET_T));
    ((SIG_LIGHT_LIGHTNESS_SET_T *)(sig_out->payload))->opcode = LIGHTNESS_SET;
    *total_len = sizeof(SIG_LIGHT_LIGHTNESS_SET_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_lighting_lightness_status_unpack(const uint8_t *in, uint8_t in_len, uint16_t *addr, SIG_LIGHT_LIGHTNESS_STATUS_T *out)
{
    if (in == NULL || out == NULL || addr == NULL || in_len <= sizeof(SIG_MESH_RSP_T) + sizeof(uint16_t)) return -1;

    SIG_MESH_RSP_T *sig_in   = (SIG_MESH_RSP_T *)in;
    uint8_t         data_len = in_len - sizeof(SIG_MESH_RSP_T);

    *addr = sig_in->src;
    if(data_len < sizeof(SIG_LIGHT_LIGHTNESS_STATUS_T)){
        OPPLE_MEMCPY((void *)out, (void *)(sig_in->data),data_len);

        out->target_lightness = out->present_lightness;
        out->reamin_time      = 0;
    } else
        OPPLE_MEMCPY((void *)out, (void *)(sig_in->data), sizeof(SIG_LIGHT_LIGHTNESS_STATUS_T));

    return 0;
}

/* Sigmesh light - CTL */
int sigmesh_lighting_ctl_set_pack(uint16_t addr, const SIG_LIGHT_CTL_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_LIGHT_CTL_SET_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_LIGHT_CTL_SET_T));
    ((SIG_LIGHT_CTL_SET_T *)(sig_out->payload))->opcode = LIGHT_CTL_SET;
    *total_len = sizeof(SIG_LIGHT_CTL_SET_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_lighting_ctl_status_unpack(const uint8_t *in, uint8_t in_len, uint16_t *addr, SIG_LIGHT_CTL_STATUS_T *out)
{
    if (in == NULL || out == NULL || addr == NULL || in_len <= sizeof(SIG_MESH_RSP_T) + sizeof(uint16_t)) return -1;

    SIG_MESH_RSP_T *sig_in   = (SIG_MESH_RSP_T *)in;
    uint8_t         data_len = in_len - sizeof(SIG_MESH_RSP_T);

    *addr = sig_in->src;
    if(data_len < sizeof(SIG_LIGHT_CTL_STATUS_T)){
        OPPLE_MEMCPY((void *)out, (void *)(sig_in->data),data_len);

        out->target_lightness = out->present_lightness;
        out->target_temp      = out->present_temp;
        out->ramin_time       = 0;
    } else
        OPPLE_MEMCPY((void *)out, (void *)(sig_in->data), sizeof(SIG_LIGHT_CTL_STATUS_T));

    return 0;
}

/* Sigmesh light - temperature */
int sigmesh_lighting_ctl_temp_set_pack(uint16_t addr, const SIG_LIGHT_CTL_TEMP_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_LIGHT_CTL_TEMP_SET_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_LIGHT_CTL_TEMP_SET_T));
    ((SIG_LIGHT_CTL_TEMP_SET_T *)(sig_out->payload))->opcode = LIGHT_CTL_TEMP_SET;
    *total_len = sizeof(SIG_LIGHT_CTL_TEMP_SET_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_lighting_ctl_temp_status_unpack(const uint8_t *in, uint8_t in_len, uint16_t *addr, SIG_LIGHT_CTL_TEMP_STATUS_T *out)
{
    if (in == NULL || out == NULL || addr == NULL || in_len <= sizeof(SIG_MESH_RSP_T) + sizeof(uint16_t)) return -1;

    SIG_MESH_RSP_T *sig_in   = (SIG_MESH_RSP_T *)in;
    uint8_t         data_len = in_len - sizeof(SIG_MESH_RSP_T);

    *addr = sig_in->src;
    if(data_len < sizeof(SIG_LIGHT_CTL_TEMP_STATUS_T)){
        OPPLE_MEMCPY((void *)out, (void *)(sig_in->data),data_len);

        out->target_delta_uv = out->present_delta_uv;
        out->target_temp     = out->present_temp;
        out->ramin_time      = 0;
    } else
        OPPLE_MEMCPY((void *)out, (void *)(sig_in->data), sizeof(SIG_LIGHT_CTL_TEMP_STATUS_T));

    return 0;
}

/* Sigmesh scene */
int sigmesh_scene_store_pack(uint16_t addr, const SIG_SCENE_STORE_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_SCENE_STORE_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_SCENE_STORE_T));
    ((SIG_SCENE_STORE_T *)(sig_out->payload))->opcode = SCENE_STORE;
    *total_len = sizeof(SIG_SCENE_STORE_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_scene_recall_pack(uint16_t addr, const SIG_SCENE_RECALL_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_SCENE_RECALL_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_SCENE_RECALL_T));
    ((SIG_SCENE_RECALL_T *)(sig_out->payload))->opcode = SCENE_CALL;
    *total_len = sizeof(SIG_SCENE_RECALL_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_scene_delete_pack(uint16_t addr, const SIG_SCENE_DELETE_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_SCENE_DELETE_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_SCENE_DELETE_T));
    ((SIG_SCENE_DELETE_T *)(sig_out->payload))->opcode = SCENE_DELETE;
    *total_len = sizeof(SIG_SCENE_DELETE_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_scene_status_unpack(const uint8_t *in, uint8_t in_len, uint16_t *addr, SIG_SCENE_STATUS_T *out)
{
    if (in == NULL || out == NULL || addr == NULL || in_len <= sizeof(SIG_MESH_RSP_T) + sizeof(uint16_t)) return -1;

    SIG_MESH_RSP_T *sig_in   = (SIG_MESH_RSP_T *)in;
    uint8_t         data_len = in_len - sizeof(SIG_MESH_RSP_T);

    *addr = sig_in->src;
    if(data_len < sizeof(SIG_SCENE_STATUS_T)){
        OPPLE_MEMCPY((void *)out, (void *)(sig_in->data),data_len);

        out->target_scene = out->current_scene;
        out->reamin_time  = 0;
    } else
        OPPLE_MEMCPY((void *)out, (void *)(sig_in->data), sizeof(SIG_SCENE_STATUS_T));

    return 0;
}

int sigmesh_scene_reg_status_unpack(const uint8_t *in, uint8_t in_len, uint16_t *addr, SIG_SCENE_REGISTER_STATUS_T *out)
{
    if (in == NULL || out == NULL || addr == NULL || in_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_SCENE_REGISTER_STATUS_T)) return -1;

    SIG_MESH_RSP_T *sig_in = (SIG_MESH_RSP_T *)in;

    *addr = sig_in->src;
    OPPLE_MEMCPY((void *)out, (void *)(sig_in->data), sizeof(SIG_SCENE_REGISTER_STATUS_T));
    return 0;
}

/* Sigmesh config model app */
int sigmesh_cfg_model_app_bind_pack(uint16_t addr, const SIG_CFG_MODEL_APP_BIND_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_CFG_MODEL_APP_BIND_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_CFG_MODEL_APP_BIND_T));
    ((SIG_CFG_MODEL_APP_BIND_T *)(sig_out->payload))->opcode = MODE_APP_BIND;
    *total_len = sizeof(SIG_CFG_MODEL_APP_BIND_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_cfg_model_app_unbind_pack(uint16_t addr, const SIG_CFG_MODEL_APP_UNBIND_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_CFG_MODEL_APP_UNBIND_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_CFG_MODEL_APP_UNBIND_T));
    *total_len = sizeof(SIG_CFG_MODEL_APP_UNBIND_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_cfg_model_app_status_unpack(const uint8_t *in, uint8_t in_len, uint16_t *addr, SIG_CFG_MODEL_APP_STATUS_T *out)
{
    if (in == NULL || out == NULL || addr == NULL || in_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_CFG_MODEL_APP_STATUS_T)) return -1;

    SIG_MESH_RSP_T *sig_in = (SIG_MESH_RSP_T *)in;

    *addr = sig_in->src;
    OPPLE_MEMCPY((void *)out, (void *)(sig_in->data), sizeof(SIG_CFG_MODEL_APP_STATUS_T));
    return 0;
}

/* Sigmesh config model subscription */
int sigmesh_cfg_model_sub_add_pack(uint16_t addr, const SIG_CFG_MODEL_SUB_ADD_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_CFG_MODEL_SUB_ADD_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_CFG_MODEL_SUB_ADD_T));
    ((SIG_CFG_MODEL_SUB_ADD_T *)(sig_out->payload))->opcode = CFG_MODEL_SUB_ADD;
    *total_len = sizeof(SIG_CFG_MODEL_SUB_ADD_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}
int sigmesh_cfg_model_sub_delete_pack(uint16_t addr, const SIG_CFG_MODEL_SUB_DELETE_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_CFG_MODEL_SUB_DELETE_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_CFG_MODEL_SUB_DELETE_T));
    ((SIG_CFG_MODEL_SUB_DELETE_T *)(sig_out->payload))->opcode = CFG_MODEL_SUB_DEL;
    *total_len = sizeof(SIG_CFG_MODEL_SUB_DELETE_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_cfg_model_sub_overwrite_pack(uint16_t addr, const SIG_CFG_MODEL_SUB_OVERWRITE_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_CFG_MODEL_SUB_OVERWRITE_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_CFG_MODEL_SUB_OVERWRITE_T));
    ((SIG_CFG_MODEL_SUB_OVERWRITE_T *)(sig_out->payload))->opcode = CFG_MODEL_SUB_OVER_WRITE;
    *total_len = sizeof(SIG_CFG_MODEL_SUB_OVERWRITE_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_cfg_model_sub_get_pack(uint16_t addr, const SIG_CFG_MODEL_SUB_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_CFG_MODEL_SUB_GET_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_CFG_MODEL_SUB_GET_T));
    ((SIG_CFG_MODEL_SUB_GET_T *)(sig_out->payload))->opcode = CFG_MODEL_SUB_GET;
    *total_len = sizeof(SIG_CFG_MODEL_SUB_GET_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_cfg_model_sub_status_unpack(const uint8_t *in, uint8_t in_len, uint16_t *addr, SIG_CFG_MODEL_SUB_STATUS_T *out)
{
    if (in == NULL || out == NULL || addr == NULL || in_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_CFG_MODEL_SUB_STATUS_T)) return -1;

    SIG_MESH_RSP_T *sig_in = (SIG_MESH_RSP_T *)in;

    *addr = sig_in->src;
    OPPLE_MEMCPY((void *)out, (void *)(sig_in->data), sizeof(SIG_CFG_MODEL_SUB_STATUS_T));
    return 0;
}

/* Sigmesh config model publication */
int sigmesh_cfg_model_pub_set_pack(uint16_t addr, const SIG_CFG_MODEL_PUB_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_CFG_MODEL_PUB_SET_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_CFG_MODEL_PUB_SET_T));
    ((SIG_CFG_MODEL_PUB_SET_T *)(sig_out->payload))->opcode = CFG_MODEL_PUB_SET;
    *total_len = sizeof(SIG_CFG_MODEL_PUB_SET_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_cfg_model_pub_get_pack(uint16_t addr, const SIG_CFG_MODEL_PUB_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_CFG_MODEL_PUB_GET_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_CFG_MODEL_PUB_GET_T));
    ((SIG_CFG_MODEL_PUB_GET_T *)(sig_out->payload))->opcode = CFG_MODEL_PUB_GET;
    *total_len = sizeof(SIG_CFG_MODEL_PUB_GET_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_cfg_model_pub_status_unpack(const uint8_t *in, uint8_t in_len, uint16_t *addr, SIG_CFG_MODEL_PUB_STATUS_T *out)
{
    if (in == NULL || out == NULL || addr == NULL || in_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_CFG_MODEL_PUB_STATUS_T)) return -1;

    SIG_MESH_RSP_T *sig_in = (SIG_MESH_RSP_T *)in;

    *addr = sig_in->src;
    OPPLE_MEMCPY((void *)out, (void *)(sig_in->data), sizeof(SIG_CFG_MODEL_PUB_STATUS_T));
    return 0;
}

/* Sigmesh config model app key */
int sigmesh_cfg_model_app_key_add_pack(uint16_t addr, const SIG_CFG_APP_KEY_ADD_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_CFG_APP_KEY_ADD_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_CFG_APP_KEY_ADD_T));
    ((SIG_CFG_APP_KEY_ADD_T *)(sig_out->payload))->opcode = APPKEY_ADD;
    *total_len = sizeof(SIG_CFG_APP_KEY_ADD_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_cfg_model_app_key_get_pack(uint16_t addr, const SIG_CFG_APP_KEY_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_CFG_APP_KEY_GET_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_CFG_APP_KEY_GET_T));
    ((SIG_CFG_APP_KEY_GET_T *)(sig_out->payload))->opcode = APPKEY_GET;
    *total_len = sizeof(SIG_CFG_APP_KEY_GET_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_cfg_model_app_key_delete_pack(uint16_t addr, const SIG_CFG_APP_KEY_DELETE_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_CFG_APP_KEY_DELETE_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_CFG_APP_KEY_DELETE_T));
    ((SIG_CFG_APP_KEY_DELETE_T *)(sig_out->payload))->opcode = APPKEY_DEL;
    *total_len = sizeof(SIG_CFG_APP_KEY_DELETE_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_cfg_app_key_status_unpack(const uint8_t *in, uint8_t in_len, uint16_t *addr, SIG_CFG_APP_KEY_STATUS_T *out)
{
    if (in == NULL || out == NULL || addr == NULL || in_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_CFG_APP_KEY_STATUS_T)) return -1;

    SIG_MESH_RSP_T *sig_in = (SIG_MESH_RSP_T *)in;

    *addr = sig_in->src;
    OPPLE_MEMCPY((void *)out, (void *)(sig_in->data), sizeof(SIG_CFG_APP_KEY_STATUS_T));
    return 0;
}

/* Sigmesh config node reset */
int sigmesh_cfg_node_reset_pack(uint16_t addr, const SIG_CFG_NODE_RESET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_CFG_NODE_RESET_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_CFG_NODE_RESET_T));
    ((SIG_CFG_NODE_RESET_T *)(sig_out->payload))->opcode = NODE_RESET;
    *total_len = sizeof(SIG_CFG_NODE_RESET_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_cfg_node_reset_status_unpack(const uint8_t *in, uint8_t in_len, uint16_t *addr, SIG_CFG_NODE_RESET_STATUS_T *out)
{
    if (in == NULL || out == NULL || addr == NULL || in_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_CFG_NODE_RESET_STATUS_T)) return -1;

    SIG_MESH_RSP_T *sig_in = (SIG_MESH_RSP_T *)in;

    *addr = sig_in->src;
    OPPLE_MEMCPY((void *)out, (void *)(sig_in->data), sizeof(SIG_CFG_NODE_RESET_STATUS_T));
    return 0;
}

/* Sigmesh config heartbeat */
int sigmesh_cfg_heartbeat_pub_set_pack(uint16_t addr, const SIG_CFG_HEARTBEAT_PUB_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_CFG_HEARTBEAT_PUB_SET_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_CFG_HEARTBEAT_PUB_SET_T));
    ((SIG_CFG_HEARTBEAT_PUB_SET_T *)(sig_out->payload))->opcode = HEARTBEAT_PUB_SET;
    *total_len = sizeof(SIG_CFG_HEARTBEAT_PUB_SET_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

/* Sigmesh node 之后会弃用*/
int sigmesh_node_status_get_pack(uint16_t addr, const SIG_NODE_STATUS_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_NODE_STATUS_GET_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_NODE_STATUS_GET_T));
    *total_len = sizeof(SIG_NODE_STATUS_GET_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

/**
 * @name:
 * @description:
 * @param {uint16_t} addr
 * @param {SIG_GENERIC_ONOFF_GET_T} *input
 * @param {uint8_t} *output
 * @param {uint16_t} out_len
 * @param {uint16_t} *total_len
 * @return {*}
 */
int sigmesh_node_g_onoff_get_pack(uint16_t addr, const SIG_GENERIC_ONOFF_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_GENERIC_ONOFF_GET_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_GENERIC_ONOFF_GET_T));
    ((SIG_GENERIC_ONOFF_GET_T *)(sig_out->payload))->opcode = G_ONOFF_GET;
    *total_len = sizeof(SIG_GENERIC_ONOFF_GET_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}
/**
 * @name:
 * @description:
 * @param {uint16_t} addr
 * @param {constLIGHT_LIGHTNESS_GET_T} *input
 * @param {uint8_t} *output
 * @param {uint16_t} out_len
 * @param {uint16_t} *total_len
 * @return {*}
 */
int sigmesh_node_lightness_get_pack(uint16_t addr, const SIG_LIGHT_LIGHTNESS_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_LIGHT_LIGHTNESS_GET_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_LIGHT_LIGHTNESS_GET_T));
    ((SIG_LIGHT_LIGHTNESS_GET_T *)(sig_out->payload))->opcode = LIGHTNESS_GET;
    *total_len = sizeof(SIG_LIGHT_LIGHTNESS_GET_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

/**
 * @name:
 * @description:
 * @param {uint16_t} addr
 * @param {constLIGHT_CTL_TEMP_GET_T} *input
 * @param {uint8_t} *output
 * @param {uint16_t} out_len
 * @param {uint16_t} *total_len
 * @return {*}
 */
int sigmesh_node_ctl_temp_get_pack(uint16_t addr, const SIG_LIGHT_CTL_TEMP_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_LIGHT_CTL_TEMP_GET_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_LIGHT_CTL_TEMP_GET_T));
    ((SIG_LIGHT_CTL_TEMP_GET_T *)(sig_out->payload))->opcode = LIGHT_CTL_TEMP_GET;
    *total_len = sizeof(SIG_LIGHT_CTL_TEMP_GET_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}
/**
 * @name:
 * @description:
 * @param {uint16_t} addr
 * @param {constLIGHT_CTL_GET_T} *input
 * @param {uint8_t} *output
 * @param {uint16_t} out_len
 * @param {uint16_t} *total_len
 * @return {*}
 */
int sigmesh_node_ctl_get_pack(uint16_t addr, const SIG_LIGHT_CTL_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(SIG_LIGHT_CTL_GET_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(SIG_LIGHT_CTL_GET_T));
    ((SIG_LIGHT_CTL_GET_T *)(sig_out->payload))->opcode = LIGHT_CTL_GET;
    *total_len = sizeof(SIG_LIGHT_CTL_GET_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

/* opple ifttt */
int sigmesh_opple_ifttt_add_pack(uint16_t addr, const OPPLE_IFTTT_ADD_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(OPPLE_IFTTT_ADD_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(OPPLE_IFTTT_ADD_T));
    ((OPPLE_IFTTT_ADD_T *)(sig_out->payload))->msg_id = G_VENDER_ATTR_IFTTT_ADD;
    *total_len = sizeof(OPPLE_IFTTT_ADD_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_opple_ifttt_delete_pack(uint16_t addr, const OPPLE_IFTTT_DELETE_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(OPPLE_IFTTT_DELETE_T) + sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);
    OPPLE_MEMCPY((void *)(sig_out->payload), (void *)input, sizeof(OPPLE_IFTTT_DELETE_T));
    ((OPPLE_IFTTT_DELETE_T *)(sig_out->payload))->msg_id = G_VENDER_ATTR_IFTTT_DEL;
    *total_len = sizeof(OPPLE_IFTTT_DELETE_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}
/********************************************************************************************************
                                    SIG MESH VENDOR - PACK
********************************************************************************************************/
/* Sigmesh vendor head packing */
static int sigmesh_vendor_get_head_packing(SIG_VENDOR_MSG_T *vendor_pack)
{
    if (vendor_pack == NULL) return -1;

    vendor_pack->opcode    = VENDOR_OPCODE_GET;
    vendor_pack->vendor_id = VENDOR_ID_OPPLE;
    vendor_pack->tid       = 0;
}

static int sigmesh_vendor_set_head_packing(SIG_VENDOR_MSG_T *vendor_pack)
{
    if (vendor_pack == NULL) return -1;

    vendor_pack->opcode    = VENDOR_OPCODE_SET_WITH_ACK;
    vendor_pack->vendor_id = VENDOR_ID_OPPLE;
    vendor_pack->tid       = 0;
}

/* Sigmesh vendor light - colour mode */
int sigmesh_vendor_light_colour_mode_get_pack(uint16_t addr, const VENDOR_LIGHT_COLOUR_MODE_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_LIGHT_COLOUR_MODE_GET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_get_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_LIGHT_COLOUR_MODE_GET_T));
    ((VENDOR_LIGHT_COLOUR_MODE_GET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_LIGHT_COLOUR_MODE;
    *total_len = sizeof(VENDOR_LIGHT_COLOUR_MODE_GET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_light_colour_mode_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_LIGHT_COLOUR_MODE_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_LIGHT_COLOUR_MODE_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_LIGHT_COLOUR_MODE_STATUS_T));
    return 0;
}

/* Sigmesh vendor light - colour */
int sigmesh_vendor_light_colour_get_pack(uint16_t addr, const VENDOR_LIGHT_COLOUR_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_LIGHT_COLOUR_GET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_get_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_LIGHT_COLOUR_GET_T));
    ((VENDOR_LIGHT_COLOUR_GET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_LIGHT_COLOUR;
    *total_len = sizeof(VENDOR_LIGHT_COLOUR_GET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_light_colour_set_pack(uint16_t addr, const VENDOR_LIGHT_COLOUR_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_LIGHT_COLOUR_SET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_set_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_LIGHT_COLOUR_SET_T));
    ((VENDOR_LIGHT_COLOUR_SET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_LIGHT_COLOUR;
    *total_len = sizeof(VENDOR_LIGHT_COLOUR_SET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_light_colour_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_LIGHT_COLOUR_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_LIGHT_COLOUR_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_LIGHT_COLOUR_STATUS_T));
    return 0;
}

/* Sigmesh vendor scene panel */
int sigmesh_vendor_scene_panel_key_num_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_SCENE_PANEL_KEY_NUM_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_SCENE_PANEL_KEY_NUM_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_SCENE_PANEL_KEY_NUM_STATUS_T));
    return 0;
}

/* Sigmesh vendor curtain - mode*/
int sigmesh_vendor_curtain_mode_get_pack(uint16_t addr, const VENDOR_CURTAIN_MODE_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_CURTAIN_MODE_GET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_get_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_CURTAIN_MODE_GET_T));
    ((VENDOR_CURTAIN_MODE_GET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_CURTAIN_MODE;
    *total_len = sizeof(VENDOR_CURTAIN_MODE_GET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_curtain_mode_set_pack(uint16_t addr, const VENDOR_CURTAIN_MODE_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_CURTAIN_MODE_SET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_set_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_CURTAIN_MODE_SET_T));
    ((VENDOR_CURTAIN_MODE_SET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_CURTAIN_MODE;
    *total_len = sizeof(VENDOR_CURTAIN_MODE_SET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_curtain_mode_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_CURTAIN_MODE_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_CURTAIN_MODE_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_CURTAIN_MODE_STATUS_T));
    return 0;
}

/* Sigmesh vendor curtain - present percent */
int sigmesh_vendor_curtain_present_percent_get_pack(uint16_t addr, const VENDOR_CURTAIN_PRESENT_PERCENT_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_CURTAIN_PRESENT_PERCENT_GET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_get_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_CURTAIN_PRESENT_PERCENT_GET_T));
    ((VENDOR_CURTAIN_PRESENT_PERCENT_GET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_CURTAIN_PRESENT_PERCENT;
    *total_len = sizeof(VENDOR_CURTAIN_PRESENT_PERCENT_GET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_curtain_present_percent_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_CURTAIN_PRESENT_PERCENT_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_CURTAIN_PRESENT_PERCENT_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_CURTAIN_PRESENT_PERCENT_STATUS_T));
    return 0;
}

/* Sigmesh vendor curtain - target percent */
int sigmesh_vendor_curtain_target_percent_get_pack(uint16_t addr, const VENDOR_CURTAIN_TARGET_PERCENT_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_CURTAIN_TARGET_PERCENT_GET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_get_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_CURTAIN_TARGET_PERCENT_GET_T));
    ((VENDOR_CURTAIN_TARGET_PERCENT_GET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_CURTAIN_TARGET_PERCENT;
    *total_len = sizeof(VENDOR_CURTAIN_TARGET_PERCENT_GET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_curtain_target_percent_set_pack(uint16_t addr, const VENDOR_CURTAIN_TARGET_PERCENT_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_CURTAIN_TARGET_PERCENT_SET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_set_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_CURTAIN_TARGET_PERCENT_SET_T));
    ((VENDOR_CURTAIN_TARGET_PERCENT_SET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_CURTAIN_TARGET_PERCENT;
    *total_len = sizeof(VENDOR_CURTAIN_TARGET_PERCENT_SET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_curtain_target_percent_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_CURTAIN_TARGET_PERCENT_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_CURTAIN_TARGET_PERCENT_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_CURTAIN_TARGET_PERCENT_STATUS_T));
    return 0;
}

/* Sigmesh vendor audible and visual alarm - light switch */
int sigmesh_vendor_aud_vis_alarm_light_switch_get_pack(uint16_t addr, const VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_GET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_get_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_GET_T));
    ((VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_GET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_AUD_VIS_ALARM_LIGHT_SWITCH;
    *total_len = sizeof(VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_GET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_aud_vis_alarm_light_switch_set_pack(uint16_t addr, const VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_SET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_set_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_SET_T));
    ((VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_SET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_AUD_VIS_ALARM_LIGHT_SWITCH;
    *total_len = sizeof(VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_SET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_aud_vis_alarm_light_switch_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_STATUS_T));
    return 0;
}

/* Sigmesh vendor audible and visual alarm - time */
int sigmesh_vendor_aud_vis_alarm_time_get_pack(uint16_t addr, const VENDOR_AUD_VIS_ALARM_TIME_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_AUD_VIS_ALARM_TIME_GET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_get_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_AUD_VIS_ALARM_TIME_GET_T));
    ((VENDOR_AUD_VIS_ALARM_TIME_GET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_AUD_VIS_ALARM_TIME;
    *total_len = sizeof(VENDOR_AUD_VIS_ALARM_TIME_GET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_aud_vis_alarm_time_set_pack(uint16_t addr, const VENDOR_AUD_VIS_ALARM_TIME_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_AUD_VIS_ALARM_TIME_SET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_set_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_AUD_VIS_ALARM_TIME_SET_T));
    ((VENDOR_AUD_VIS_ALARM_TIME_SET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_AUD_VIS_ALARM_TIME;
    *total_len = sizeof(VENDOR_AUD_VIS_ALARM_TIME_SET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_aud_vis_alarm_time_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_AUD_VIS_ALARM_TIME_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_AUD_VIS_ALARM_TIME_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_AUD_VIS_ALARM_TIME_STATUS_T));
    return 0;
}

/* Sigmesh vendor audible and visual alarm - volume */
int sigmesh_vendor_aud_vis_alarm_volume_get_pack(uint16_t addr, const VENDOR_AUD_VIS_ALARM_VOLUME_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_AUD_VIS_ALARM_VOLUME_GET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_get_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_AUD_VIS_ALARM_VOLUME_GET_T));
    ((VENDOR_AUD_VIS_ALARM_VOLUME_GET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_AUD_VIS_ALARM_VOLUME;
    *total_len = sizeof(VENDOR_AUD_VIS_ALARM_VOLUME_GET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_aud_vis_alarm_volume_set_pack(uint16_t addr, const VENDOR_AUD_VIS_ALARM_VOLUME_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_AUD_VIS_ALARM_VOLUME_SET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_set_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_AUD_VIS_ALARM_VOLUME_SET_T));
    ((VENDOR_AUD_VIS_ALARM_VOLUME_SET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_AUD_VIS_ALARM_VOLUME;
    *total_len = sizeof(VENDOR_AUD_VIS_ALARM_VOLUME_SET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_aud_vis_alarm_volume_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_AUD_VIS_ALARM_VOLUME_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_AUD_VIS_ALARM_VOLUME_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_AUD_VIS_ALARM_VOLUME_STATUS_T));
    return 0;
}

/* Sigmesh vendor sensor alarm */
int sigmesh_vendor_sensor_dismantle_alarm_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_SENSOR_DISMANTLE_ALARM_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_SENSOR_DISMANTLE_ALARM_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_SENSOR_DISMANTLE_ALARM_T));
    return 0;
}

int sigmesh_vendor_sensor_alarm_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_SENSOR_ALARM_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_SENSOR_ALARM_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_SENSOR_ALARM_STATUS_T));
    return 0;
}

/* Sigmesh vendor PIR sensor - event */
int sigmesh_vendor_pir_sensor_event_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_PIR_SENSOR_EVENT_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_PIR_SENSOR_EVENT_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_PIR_SENSOR_EVENT_STATUS_T));
    return 0;
}

/* Sigmesh vendor PIR sensor - status */
int sigmesh_vendor_pir_sensor_status_get_pack(uint16_t addr, const VENDOR_PIR_SENSOR_STATUS_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_PIR_SENSOR_STATUS_GET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_get_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_PIR_SENSOR_STATUS_GET_T));
    ((VENDOR_PIR_SENSOR_STATUS_GET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_PIR_SENSOR_STATUS;
    *total_len = sizeof(VENDOR_PIR_SENSOR_STATUS_GET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_pir_sensor_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_PIR_SENSOR_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_PIR_SENSOR_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_PIR_SENSOR_STATUS_T));
    return 0;
}

/* Sigmesh vendor door sensor - event */
int sigmesh_vendor_door_sensor_event_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_DOOR_SENSOR_EVENT_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_DOOR_SENSOR_EVENT_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_DOOR_SENSOR_EVENT_STATUS_T));
    return 0;
}

/* Sigmesh vendor door sensor - status */
int sigmesh_vendor_door_sensor_status_get_pack(uint16_t addr, const VENDOR_DOOR_SENSOR_STATUS_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_DOOR_SENSOR_STATUS_GET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_get_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_DOOR_SENSOR_STATUS_GET_T));
    ((VENDOR_DOOR_SENSOR_STATUS_GET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_DOOR_SENSOR_STATUS;
    *total_len = sizeof(VENDOR_DOOR_SENSOR_STATUS_GET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_door_sensor_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_DOOR_SENSOR_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_DOOR_SENSOR_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_DOOR_SENSOR_STATUS_T));
    return 0;
}

/* Sigmesh vendor temperature and humidity sensor - temperature */
int sigmesh_vendor_tem_hum_sensor_tem_get_pack(uint16_t addr, const VENDOR_TEM_HUM_SENSOR_TEM_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_TEM_HUM_SENSOR_TEM_GET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_get_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_TEM_HUM_SENSOR_TEM_GET_T));
    ((VENDOR_TEM_HUM_SENSOR_TEM_GET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_TEM_HUM_SENSOR_TEM;
    *total_len = sizeof(VENDOR_TEM_HUM_SENSOR_TEM_GET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_tem_hum_sensor_tem_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_TEM_HUM_SENSOR_TEM_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_TEM_HUM_SENSOR_TEM_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_TEM_HUM_SENSOR_TEM_STATUS_T));
    return 0;
}

int sigmesh_vendor_tem_hum_sensor_tem_level_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_TEM_HUM_SENSOR_TEM_LEVEL_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_TEM_HUM_SENSOR_TEM_LEVEL_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_TEM_HUM_SENSOR_TEM_LEVEL_STATUS_T));
    return 0;
}

/* Sigmesh vendor temperature and humidity sensor - humidity */
int sigmesh_vendor_tem_hum_sensor_hum_get_pack(uint16_t addr, const VENDOR_TEM_HUM_SENSOR_HUM_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_TEM_HUM_SENSOR_HUM_GET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_get_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_TEM_HUM_SENSOR_HUM_GET_T));
    ((VENDOR_TEM_HUM_SENSOR_HUM_GET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_TEM_HUM_SENSOR_HUM;
    *total_len = sizeof(VENDOR_TEM_HUM_SENSOR_HUM_GET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_tem_hum_sensor_hum_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_TEM_HUM_SENSOR_HUM_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_TEM_HUM_SENSOR_HUM_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_TEM_HUM_SENSOR_HUM_STATUS_T));
    return 0;
}

int sigmesh_vendor_tem_hum_sensor_hum_level_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_TEM_HUM_SENSOR_HUM_LEVEL_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_TEM_HUM_SENSOR_HUM_LEVEL_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_TEM_HUM_SENSOR_HUM_LEVEL_STATUS_T));
    return 0;
}

/* Sigmesh vendor water sensor */
int sigmesh_vendor_water_sensor_alarm_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_WATER_SENSOR_ALARM_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_WATER_SENSOR_ALARM_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_WATER_SENSOR_ALARM_STATUS_T));
    return 0;
}

/* Sigmesh vendor device battery - level */
int sigmesh_vendor_dev_battery_level_get_pack(uint16_t addr, const VENDOR_DEV_BATTERY_LEVEL_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_DEV_BATTERY_LEVEL_GET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_get_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_DEV_BATTERY_LEVEL_GET_T));
    ((VENDOR_DEV_BATTERY_LEVEL_GET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_BATTERY_LEVEL;
    *total_len = sizeof(VENDOR_DEV_BATTERY_LEVEL_GET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_dev_battery_level_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_DEV_BATTERY_LEVEL_STATUS_T *out)
{
    if (in == NULL || out == NULL ||in_len < sizeof(VENDOR_DEV_BATTERY_LEVEL_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_DEV_BATTERY_LEVEL_STATUS_T));
    return 0;
}

/* Sigmesh vendor device battery - alarm */
int sigmesh_vendor_dev_battery_alarm_get_pack(uint16_t addr, const VENDOR_DEV_BATTERY_ALARM_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len)
{
    if (addr == 0
       || out_len < sizeof(VENDOR_DEV_BATTERY_ALARM_GET_T) + sizeof(SIG_VENDOR_MSG_T) +sizeof(SIG_MESH_CMD_T)
       || input == NULL
       || output == NULL) {
        return -1;
    }

    SIG_MESH_CMD_T *sig_out = (SIG_MESH_CMD_T *)output;
    sig_cmd_head_packing(addr, sig_out);

    SIG_VENDOR_MSG_T *vendor_out = (SIG_VENDOR_MSG_T *)sig_out->payload;
    sigmesh_vendor_get_head_packing(vendor_out);
    OPPLE_MEMCPY((void *)(vendor_out->data), (void *)input, sizeof(VENDOR_DEV_BATTERY_ALARM_GET_T));
    ((VENDOR_DEV_BATTERY_ALARM_GET_T *)(vendor_out->data))->attr_type = VENDOR_ATTR_BATTERY_ALARM;
    *total_len = sizeof(VENDOR_DEV_BATTERY_ALARM_GET_T) + sizeof(SIG_VENDOR_MSG_T) + sizeof(SIG_MESH_CMD_T);
    return 0;
}

int sigmesh_vendor_dev_battery_alarm_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_DEV_BATTERY_ALARM_STATUS_T *out)
{
    if (in == NULL || out == NULL ||in_len < sizeof(VENDOR_DEV_BATTERY_ALARM_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_DEV_BATTERY_ALARM_STATUS_T));
    return 0;
}

/* Sigmesh vendor device battery - capacity */
int sigmesh_vendor_dev_battery_capacity_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_DEV_BATTERY_CAPACITY_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_DEV_BATTERY_CAPACITY_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_DEV_BATTERY_CAPACITY_STATUS_T));
    return 0;
}

/* Sigmesh vendor device battery - charging */
int sigmesh_vendor_dev_battery_charging_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_DEV_BATTERY_CHARGING_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_DEV_BATTERY_CHARGING_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_DEV_BATTERY_CHARGING_STATUS_T));
    return 0;
}

/*vendor version*/
int sigmesh_vendor_dev_version_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_DEV_VERSION_STATUS_T *out)
{
    if (in == NULL || out == NULL || in_len < sizeof(VENDOR_DEV_VERSION_STATUS_T)) return -1;

    OPPLE_MEMCPY((void *)out, (void *)in, sizeof(VENDOR_DEV_VERSION_STATUS_T));
    return 0;
}

/*******************************************************************************************************
                             SIG MESH MESSAGE TO OPPLE MESH MESSAGE
********************************************************************************************************/
/* Light onoff*/
static int sig_2_opple_onoff_set_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_GENERIC_ONOFF_SET_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_LIGHT_ONOFF_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T          *sig_msg     = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T        *opple_msg   = (OPPLE_MESH_MSG_T *)out->data;
    SIG_GENERIC_ONOFF_SET_T *sig_onoff   = (SIG_GENERIC_ONOFF_SET_T *)(sig_msg->data);
    OPPLE_LIGHT_ONOFF_T     *light_onoff = (OPPLE_LIGHT_ONOFF_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_ONOFF;
    opple_msg->group_number = dnlink1_calc_gpno(sig_msg->src,OPP_LIGHT_GPNB_HASH_OBJ_ID);
    light_onoff->onoff      = sig_onoff->onoff;

    //jx_hal_log_notice("opp onoff set:%d",opple_lightness->lightness);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_LIGHT_ONOFF_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_onoff_set_rsp_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_GENERIC_RSP_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T      *sig_msg   = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T    *opple_msg = (OPPLE_MESH_MSG_T *)out->data;
    OPPLE_GENERIC_RSP_T *opple_rsp = (OPPLE_GENERIC_RSP_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_ONOFF_RSP;
    opple_msg->group_number = dnlink1_calc_gpno(sig_msg->src,OPP_LIGHT_GPNB_HASH_OBJ_ID);
    opple_rsp->opcode       = OPPLE_SUCCESS;

    //jx_hal_log_notice("opp onoff rsp");

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_GENERIC_RSP_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_onoff_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    OPPLE_DEV_NOTIFY           *opple_msg         = (OPPLE_DEV_NOTIFY *)out->data;
    OPPLE_LIGHT_NOTIFY_PARA_T  *light_notify_para = (OPPLE_LIGHT_NOTIFY_PARA_T *)(opple_msg->para);
    SIG_GENERIC_ONOFF_STATUS_T  sig_onoff_status;

    if(sigmesh_generic_onoff_status_unpack(in->data,in->data_len,&out->addr,&sig_onoff_status) != 0) return -1;

    opple_msg->state             = OPPLE_STATE_NORMAL;
    light_notify_para->onoff     = sig_onoff_status.target_onoff;
    light_notify_para->ligntness = OPPLE_LIGHT_LIGHTNESS_INVALID;
    light_notify_para->cct       = OPPLE_LIGHT_CCT_INVALID;

    //jx_hal_log_notice("opp onoff:%d",light_notify_para->onoff);

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    return 0;
}

/* Lightness */
static int sig_2_opple_lightness_set_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_LIGHT_LIGHTNESS_SET_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_LIGHTNESS_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T            *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T          *opple_msg       = (OPPLE_MESH_MSG_T *)out->data;
    SIG_LIGHT_LIGHTNESS_SET_T *sig_lightness   = (SIG_LIGHT_LIGHTNESS_SET_T *)(sig_msg->data);
    OPPLE_LIGHTNESS_T         *opple_lightness = (OPPLE_LIGHTNESS_T *)(opple_msg->data);

    opple_msg->msg_id          = OPPLE_MSG_MODIFY_BRIGHT;
    opple_msg->group_number    = dnlink1_calc_gpno(sig_msg->src,OPP_LIGHT_GPNB_HASH_OBJ_ID);
    opple_lightness->lightness = M_OPP_NTOHS(sig_lightness->lightness * 255 / 65535);

    //jx_hal_log_notice("opp lightness set:%d",opple_lightness->lightness);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_LIGHTNESS_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_lightness_set_rsp_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_GENERIC_RSP_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T       *sig_msg   = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T     *opple_msg = (OPPLE_MESH_MSG_T *)out->data;
    OPPLE_GENERIC_RSP_T  *opple_rsp = (OPPLE_GENERIC_RSP_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_MODIFY_BRIGHT_RSP;
    opple_msg->group_number = dnlink1_calc_gpno(sig_msg->src,OPP_LIGHT_GPNB_HASH_OBJ_ID);
    opple_rsp->opcode       = OPPLE_SUCCESS;

    //jx_hal_log_notice("opp lightness set rsp");

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_GENERIC_RSP_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_lightness_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    OPPLE_DEV_NOTIFY             *opple_msg            = (OPPLE_DEV_NOTIFY *)out->data;
    OPPLE_LIGHT_NOTIFY_PARA_T    *light_notify_para    = (OPPLE_LIGHT_NOTIFY_PARA_T *)(opple_msg->para);
    SIG_LIGHT_LIGHTNESS_STATUS_T  sig_lightness_status;

    if(sigmesh_lighting_lightness_status_unpack(in->data,in->data_len,&out->addr,&sig_lightness_status) != 0) return -1;

    opple_msg->state             = OPPLE_STATE_NORMAL;
    light_notify_para->onoff     = OPPLE_LIGHT_ONOFF_INVALID;
    light_notify_para->ligntness = M_OPP_NTOHS(sig_lightness_status.target_lightness * 255 / 65535);
    light_notify_para->cct       = OPPLE_LIGHT_CCT_INVALID;

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    return 0;
}

/* Light color temperature */
static int sig_2_opple_light_cct_set_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_LIGHT_CTL_TEMP_SET_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_LIGHT_CCT_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T           *sig_msg      = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T         *opple_msg    = (OPPLE_MESH_MSG_T *)out->data;
    SIG_LIGHT_CTL_TEMP_SET_T *sig_ctl_temp = (SIG_LIGHT_CTL_TEMP_SET_T *)(sig_msg->data);
    OPPLE_LIGHT_CCT_T        *light_cct    = (OPPLE_LIGHT_CCT_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_MODIFY_CCT;
    opple_msg->group_number = dnlink1_calc_gpno(sig_msg->src,OPP_LIGHT_GPNB_HASH_OBJ_ID);
    light_cct->cct          = M_OPP_NTOHS(sig_ctl_temp->temperature);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_LIGHT_CCT_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_light_cct_set_rsp_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_GENERIC_RSP_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T      *sig_msg   = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T    *opple_msg = (OPPLE_MESH_MSG_T *)out->data;
    OPPLE_GENERIC_RSP_T *opple_rsp = (OPPLE_GENERIC_RSP_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_MODIFY_CCT_RSP;
    opple_msg->group_number = dnlink1_calc_gpno(sig_msg->src,OPP_LIGHT_GPNB_HASH_OBJ_ID);
    opple_rsp->opcode       = OPPLE_SUCCESS;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_GENERIC_RSP_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_light_cct_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    OPPLE_DEV_NOTIFY            *opple_msg         = (OPPLE_DEV_NOTIFY *)out->data;
    OPPLE_LIGHT_NOTIFY_PARA_T   *light_notify_para = (OPPLE_LIGHT_NOTIFY_PARA_T *)(opple_msg->para);
    SIG_LIGHT_CTL_TEMP_STATUS_T  sig_cct_status;

    if(sigmesh_lighting_ctl_temp_status_unpack(in->data,in->data_len,&out->addr,&sig_cct_status) != 0) return -1;

    opple_msg->state             = OPPLE_STATE_NORMAL;
    light_notify_para->onoff     = OPPLE_LIGHT_ONOFF_INVALID;
    light_notify_para->ligntness = OPPLE_LIGHT_LIGHTNESS_INVALID;

    uint16_t stateVal = sig_cct_status.target_temp;

    light_notify_para->cct       = M_OPP_NTOHS(stateVal);

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    return 0;
}

static int sig_2_opple_lightness_cct_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    OPPLE_DEV_NOTIFY          *opple_msg         = (OPPLE_DEV_NOTIFY *)out->data;
    OPPLE_LIGHT_NOTIFY_PARA_T *light_notify_para = (OPPLE_LIGHT_NOTIFY_PARA_T *)(opple_msg->para);
    SIG_LIGHT_CTL_STATUS_T     sig_ctl_status;

    if(sigmesh_lighting_ctl_status_unpack(in->data,in->data_len,&out->addr,&sig_ctl_status) != 0) return -1;

    opple_msg->state             = OPPLE_STATE_NORMAL;
    light_notify_para->onoff     = OPPLE_LIGHT_ONOFF_INVALID;
    light_notify_para->ligntness = M_OPP_NTOHS(sig_ctl_status.target_lightness * 255 / 65535);
    
    uint16_t stateVal = sig_ctl_status.target_temp;

    light_notify_para->cct       = M_OPP_NTOHS(stateVal);

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    return 0;
}

/* device config setting response */
static int sig_2_opple_group_config_set_rsp_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_CFG_MODEL_SUB_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_GENERIC_RSP_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T             *sig_msg        = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T           *opple_msg      = (OPPLE_MESH_MSG_T *)out->data;
    SIG_CFG_MODEL_SUB_STATUS_T *sig_sub_status = (SIG_CFG_MODEL_SUB_STATUS_T *)(sig_msg->data);
    OPPLE_GENERIC_RSP_T        *opple_rsp      = (OPPLE_GENERIC_RSP_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_DEV_CONFIG_SETTING_RSP;
    opple_msg->group_number = 0;
    opple_rsp->opcode       = sig_sub_status->status ? OPPLE_FAILED : OPPLE_SUCCESS;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_GENERIC_RSP_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

/* Group */
static int sig_2_opple_group_add_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_CFG_MODEL_SUB_ADD_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_GROUP_ADD_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T          *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T        *opple_msg       = (OPPLE_MESH_MSG_T *)out->data;
    SIG_CFG_MODEL_SUB_ADD_T *sig_sub_add     = (SIG_CFG_MODEL_SUB_ADD_T *)(sig_msg->data);
    OPPLE_GROUP_ADD_T       *opple_group_add = (OPPLE_GROUP_ADD_T *)(opple_msg->data);

    opple_msg->msg_id         = OPPLE_MSG_ADD_GRP;
    opple_msg->group_number   = 0;
    opple_group_add->group_id = M_OPP_NTOHS(sig_sub_add->address);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_GROUP_ADD_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_group_add_rsp_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_CFG_MODEL_SUB_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_GENERIC_RSP_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T             *sig_msg        = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T           *opple_msg      = (OPPLE_MESH_MSG_T *)out->data;
    SIG_CFG_MODEL_SUB_STATUS_T *sig_sub_status = (SIG_CFG_MODEL_SUB_STATUS_T *)(sig_msg->data);
    OPPLE_GENERIC_RSP_T        *opple_rsp      = (OPPLE_GENERIC_RSP_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_ADD_GRP_RSP;
    opple_msg->group_number = 0;
    opple_rsp->opcode       = sig_sub_status->status ? OPPLE_FAILED : OPPLE_SUCCESS;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_GENERIC_RSP_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_group_delete_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_CFG_MODEL_SUB_DELETE_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_GROUP_DELETE_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T             *sig_msg            = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T           *opple_msg          = (OPPLE_MESH_MSG_T *)out->data;
    SIG_CFG_MODEL_SUB_DELETE_T *sig_sub_delete     = (SIG_CFG_MODEL_SUB_DELETE_T *)(sig_msg->data);
    OPPLE_GROUP_DELETE_T       *opple_group_delete = (OPPLE_GROUP_DELETE_T *)(opple_msg->data);

    opple_msg->msg_id            = OPPLE_MSG_DEL_GRP;
    opple_msg->group_number      = 0;
    opple_group_delete->group_id = M_OPP_NTOHS(sig_sub_delete->address);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_GROUP_DELETE_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_group_delete_rsp_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_CFG_MODEL_SUB_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_GENERIC_RSP_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T             *sig_msg        = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T           *opple_msg      = (OPPLE_MESH_MSG_T *)out->data;
    SIG_CFG_MODEL_SUB_STATUS_T *sig_sub_status = (SIG_CFG_MODEL_SUB_STATUS_T *)(sig_msg->data);
    OPPLE_GENERIC_RSP_T        *opple_rsp      = (OPPLE_GENERIC_RSP_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_DEL_GRP_RSP;
    opple_msg->group_number = 0;
    opple_rsp->opcode       = sig_sub_status->status ? OPPLE_FAILED : OPPLE_SUCCESS;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_GENERIC_RSP_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

/* Scene */
static int sig_2_opple_scene_store_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_SCENE_STORE_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_SCENE_STORE_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T      *sig_msg           = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T    *opple_msg         = (OPPLE_MESH_MSG_T *)out->data;
    SIG_SCENE_STORE_T   *sig_scene_store   = (SIG_SCENE_STORE_T *)(sig_msg->data);
    OPPLE_SCENE_STORE_T *opple_scene_store = (OPPLE_SCENE_STORE_T *)(opple_msg->data);

    opple_msg->msg_id            = OPPLE_MSG_ADD_SENCE;
    opple_msg->group_number      = 0;
    opple_scene_store->scene_id  = M_OPP_NTOHL((uint32_t)sig_scene_store->scene_id);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_SCENE_STORE_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_scene_store_rsp_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_SCENE_REGISTER_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_GENERIC_RSP_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T      *sig_msg                    = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T    *opple_msg                  = (OPPLE_MESH_MSG_T *)out->data;
    SIG_SCENE_REGISTER_STATUS_T  *sig_scene_status  = (SIG_SCENE_REGISTER_STATUS_T *)(sig_msg->data);
    OPPLE_GENERIC_RSP_T *opple_rsp                  = (OPPLE_GENERIC_RSP_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_ADD_SENCE_RSP;
    opple_msg->group_number = 0;
    opple_rsp->opcode       = sig_scene_status->status ? OPPLE_FAILED : OPPLE_SUCCESS;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_GENERIC_RSP_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_scene_recall_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_SCENE_RECALL_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_SCENE_RECALL_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T       *sig_msg            = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T     *opple_msg          = (OPPLE_MESH_MSG_T *)out->data;
    SIG_SCENE_RECALL_T   *sig_scene_recall   = (SIG_SCENE_RECALL_T *)(sig_msg->data);
    OPPLE_SCENE_RECALL_T *opple_scene_recall = (OPPLE_SCENE_RECALL_T *)(opple_msg->data);

    opple_msg->msg_id            = OPPLE_MSG_CALL_SENCE;
    opple_msg->group_number      = 0;
    opple_scene_recall->scene_id = M_OPP_NTOHL((uint32_t)sig_scene_recall->scene_id);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_SCENE_RECALL_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_scene_recall_rsp_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_GENERIC_RSP_T) ||
        out == NULL || out->data == NULL) return -1;

    OPPLE_MESH_MSG_T    *opple_msg         = (OPPLE_MESH_MSG_T *)out->data;
    OPPLE_GENERIC_RSP_T *opple_rsp         = (OPPLE_GENERIC_RSP_T *)(opple_msg->data);
    SIG_SCENE_STATUS_T   sig_scene_status;

    if(sigmesh_scene_status_unpack(in->data,in->data_len,&out->addr,&sig_scene_status) != 0) return -1;

    opple_msg->msg_id       = OPPLE_MSG_CALL_SENCE_RSP;
    opple_msg->group_number = 0;
    opple_rsp->opcode       = sig_scene_status.status ? OPPLE_FAILED : OPPLE_SUCCESS;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_GENERIC_RSP_T);
    out->pack_type = OPPLE_EVENT;
    return 0;
}

static int sig_2_opple_scene_delete_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_SCENE_DELETE_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_SCENE_DELETE_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T       *sig_msg            = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T     *opple_msg          = (OPPLE_MESH_MSG_T *)out->data;
    SIG_SCENE_DELETE_T   *sig_scene_delete   = (SIG_SCENE_DELETE_T *)(sig_msg->data);
    OPPLE_SCENE_DELETE_T *opple_scene_delete = (OPPLE_SCENE_DELETE_T *)(opple_msg->data);

    opple_msg->msg_id             = OPPLE_MSG_DEL_SENCE;
    opple_msg->group_number       = 0;
    opple_scene_delete->scene_id  = M_OPP_NTOHL((uint32_t)sig_scene_delete->scene_id);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_SCENE_STORE_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_scene_delete_rsp_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_SCENE_REGISTER_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_GENERIC_RSP_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T              *sig_msg          = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T            *opple_msg        = (OPPLE_MESH_MSG_T *)out->data;
    SIG_SCENE_REGISTER_STATUS_T *sig_scene_status = (SIG_SCENE_REGISTER_STATUS_T *)(sig_msg->data);
    OPPLE_GENERIC_RSP_T         *opple_rsp        = (OPPLE_GENERIC_RSP_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_DEL_SENCE_RSP;
    opple_msg->group_number = 0;
    opple_rsp->opcode       = sig_scene_status->status ? OPPLE_FAILED : OPPLE_SUCCESS;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_GENERIC_RSP_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_scene_register_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
    in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_SCENE_REGISTER_STATUS_T) ||
    out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T              *sig_msg              = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T            *opple_msg            = (OPPLE_MESH_MSG_T *)out->data;
    SIG_SCENE_REGISTER_STATUS_T *sig_scene_reg_status = (SIG_SCENE_REGISTER_STATUS_T *)(sig_msg->data);
    OPPLE_SCENE_QUERY_RSP_T     *opple_scene          = (OPPLE_SCENE_QUERY_RSP_T *)(opple_msg->data);
    uint8_t                      scene_number         = (in->data_len - sizeof(SIG_MESH_RSP_T) - sizeof(SIG_SCENE_REGISTER_STATUS_T)) / sizeof(uint16_t);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_SCENE_QUERY_RSP_T) + opple_scene->number * sizeof(uint16_t);
    if(buf_len < sizeof(OPPLE_OUT_T) + out->data_len) return -1;

    opple_msg->msg_id       = OPPLE_MSG_QUERY_SENCE_RSP;
    opple_msg->group_number = 0;
    opple_scene->status     = sig_scene_reg_status->status ? OPPLE_FAILED : OPPLE_SUCCESS;
    opple_scene->number     = scene_number;

    for(uint8_t i = 0;i < opple_scene->number;i ++){
        opple_scene->scene_id[i] = M_OPP_NTOHS(sig_scene_reg_status->scene_payload[i]);
    }

    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_scene_config_set_rsp_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_SCENE_REGISTER_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_GENERIC_RSP_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T              *sig_msg              = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T            *opple_msg            = (OPPLE_MESH_MSG_T *)out->data;
    SIG_SCENE_REGISTER_STATUS_T *sig_scene_reg_status = (SIG_SCENE_REGISTER_STATUS_T *)(sig_msg->data);
    OPPLE_GENERIC_RSP_T         *opple_rsp            = (OPPLE_GENERIC_RSP_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_DEV_CONFIG_SETTING_RSP;
    opple_msg->group_number = 0;
    opple_rsp->opcode       = OPPLE_SUCCESS;//sig_scene_reg_status->status ? OPPLE_FAILED : OPPLE_SUCCESS;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_GENERIC_RSP_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

/* Reset */
static int sig_2_opple_dev_reset_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DEV_RESET_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T       *sig_msg          = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T     *opple_msg        = (OPPLE_MESH_MSG_T *)out->data;
    OPPLE_DEV_RESET_T    *opple_dev_reset  = (OPPLE_DEV_RESET_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_FACTORY_RESET;
    opple_msg->group_number = 0;
    opple_dev_reset->param  = 0;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DEV_RESET_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_dev_reset_rsp_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_GENERIC_RSP_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T       *sig_msg   = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T     *opple_msg = (OPPLE_MESH_MSG_T *)out->data;
    OPPLE_GENERIC_RSP_T  *opple_rsp = (OPPLE_GENERIC_RSP_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_FACTORY_RESET_RSP;
    opple_msg->group_number = 0;
    opple_rsp->opcode       = OPPLE_SUCCESS;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_GENERIC_RSP_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int sig_2_opple_node_reset_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DEV_RESET_STATUS_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T           *sig_msg            = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T         *opple_msg          = (OPPLE_MESH_MSG_T *)out->data;
    OPPLE_DEV_RESET_STATUS_T *opple_reset_status = (OPPLE_DEV_RESET_STATUS_T *)(opple_msg->data);

    opple_msg->msg_id          = OPPLE_MSG_RESET_REPORT;
    opple_msg->group_number    = 0;
    opple_reset_status->status = 0x01;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DEV_RESET_STATUS_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

/* Door lock */
static int sig_2_opple_door_lock_open_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(SIG_GENERIC_ONOFF_SET_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DOOR_LOCK_OPEN_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T           *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T         *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T         *opple_msg       = (OPPLE_MESH_MSG_T *)out->data;
    SIG_GENERIC_ONOFF_SET_T  *sig_door_lock   = (SIG_GENERIC_ONOFF_SET_T *)(vendor_msg->data);
    OPPLE_DOOR_LOCK_OPEN_T   *opple_door_lock = (OPPLE_DOOR_LOCK_OPEN_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_DOOR_LOCK_OPEN;
    opple_msg->group_number = dnlink1_calc_gpno(sig_msg->src,OPP_SENSOR_GPNB_HASH_OBJ_ID);
    opple_door_lock->open   = sig_door_lock->onoff;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DOOR_LOCK_OPEN_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

/*******************************************************************************************************
                             SIG MESH VENDOR MESSAGE TO OPPLE MESH MESSAGE
********************************************************************************************************/
/* Common */
static int vendor_2_opple_device_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    
}

static int vendor_2_opple_device_version_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_VERSION_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DEV_VERSION_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T               *sig_msg       = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T             *vendor_msg    = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T             *opple_msg     = (OPPLE_MESH_MSG_T *)out->data;  
    VENDOR_DEV_VERSION_STATUS_T  *sig_dev_ver   = (VENDOR_DEV_VERSION_STATUS_T *)(vendor_msg->data);
    OPPLE_DEV_VERSION_T          *opple_dev_ver = (OPPLE_DEV_VERSION_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_DEV_UPGRADE_IND;
    opple_msg->group_number = 0;
    opple_dev_ver->version  = M_OPP_NTOHL((uint32_t)M_OPP_NTOHS(sig_dev_ver->firmware_ver));
    

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DEV_VERSION_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;

    return 0;              
}

static int vendor_2_opple_device_time_zone_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_TIME_ZONE_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DEV_UNIX_TIME_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T          *sig_msg     = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T        *vendor_msg  = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T        *opple_msg   = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_DEV_TIME_ZONE_T  *sig_time    = (VENDOR_DEV_TIME_ZONE_T *)(vendor_msg->data);
    OPPLE_DEV_UNIX_TIME_T   *opple_time  = (OPPLE_DEV_UNIX_TIME_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_SYNC_TIME;
    opple_msg->group_number = 0;
    opple_time->time_zone   = (int16_t)sig_time->time_zone;
    opple_time->time_stamp  = OPPLE_TIME_STAMP_INVALID;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DEV_UNIX_TIME_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_device_unix_time_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_UNIX_TIME_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DEV_UNIX_TIME_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T          *sig_msg     = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T        *vendor_msg  = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T        *opple_msg   = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_DEV_UNIX_TIME_T  *sig_time    = (VENDOR_DEV_UNIX_TIME_T *)(vendor_msg->data);
    OPPLE_DEV_UNIX_TIME_T   *opple_time  = (OPPLE_DEV_UNIX_TIME_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_SYNC_TIME;
    opple_msg->group_number = 0;
    opple_time->time_zone   = OPPLE_TIME_ZONE_INVALID;
    opple_time->time_stamp  = sig_time->time_stamp;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DEV_UNIX_TIME_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

/* Scene panel */
static int vendor_2_opple_scene_panel_key_report_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_SCENE_PANEL_KEY_NUM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_SCENE_PANEL_KEY_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                      *sig_msg          = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                    *vendor_msg       = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T                    *opple_msg        = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_SCENE_PANEL_KEY_NUM_STATUS_T *sig_panel_key    = (VENDOR_SCENE_PANEL_KEY_NUM_STATUS_T *)(vendor_msg->data);
    OPPLE_SCENE_PANEL_KEY_T             *opple_panel_key  = (OPPLE_SCENE_PANEL_KEY_T *)(opple_msg->data);

    opple_msg->msg_id        = OPPLE_MSG_SCENE_PANEL_KEY_REPORT;
    opple_msg->group_number  = dnlink1_calc_gpno(sig_msg->src,OPP_PANEL_GPNB_HASH_OBJ_ID);
    opple_panel_key->key_num = sig_panel_key->key_num;

    opple_panel_key->rsv[0]  = 0;
    opple_panel_key->rsv[1]  = 0;
    opple_panel_key->rsv[2]  = 0;

    jx_hal_log_notice("opp key num:%d",opple_panel_key->key_num);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_SCENE_PANEL_KEY_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_scene_panel_battery_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_BATTERY_LEVEL_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                    *sig_msg              = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                  *vendor_msg           = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                  *opple_msg            = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_DEV_BATTERY_LEVEL_STATUS_T *sig_bat_level_status = (VENDOR_DEV_BATTERY_LEVEL_STATUS_T *)(vendor_msg->data);
    OPPLE_SCENE_PANEL_NOTIFY_PARA_T   *panel_notify_para    = (OPPLE_SCENE_PANEL_NOTIFY_PARA_T *)(opple_msg->para);

    opple_msg->state                 = OPPLE_STATE_INVALID;
    panel_notify_para->battery_level = sig_bat_level_status->level;

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_scene_panel_battery_alarm_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_BATTERY_ALARM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                    *sig_msg              = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                  *vendor_msg           = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                  *opple_msg            = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_DEV_BATTERY_ALARM_STATUS_T *sig_bat_alarm_status = (VENDOR_DEV_BATTERY_ALARM_STATUS_T *)(vendor_msg->data);
    OPPLE_SCENE_PANEL_NOTIFY_PARA_T   *panel_notify_para    = (OPPLE_SCENE_PANEL_NOTIFY_PARA_T *)(opple_msg->para);

    opple_msg->state                 = sig_bat_alarm_status->alarm ? OPPLE_STATE_LOW_POWER : OPPLE_STATE_NORMAL;
    panel_notify_para->battery_level = OPPLE_BATTERY_LEVEL_INVALID;

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

/* Curtain */
static int vendor_2_opple_curtain_target_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_CURTAIN_TARGET_PERCENT_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                         *sig_msg             = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                       *vendor_msg          = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                       *opple_msg           = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_CURTAIN_TARGET_PERCENT_STATUS_T *sig_curtain_status  = (VENDOR_CURTAIN_TARGET_PERCENT_STATUS_T *)(vendor_msg->data);
    OPPLE_CURTAIN_NOTIFY_PARA_T            *curtain_notify_para = (OPPLE_CURTAIN_NOTIFY_PARA_T *)(opple_msg->para);

    opple_msg->state             = OPPLE_STATE_NORMAL;
    curtain_notify_para->percent = sig_curtain_status->percent;

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_curtain_present_set_rsp_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_GENERIC_RSP_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T      *sig_msg   = (SIG_MESH_RSP_T *)in->data;
    OPPLE_MESH_MSG_T    *opple_msg = (OPPLE_MESH_MSG_T *)out->data;
    OPPLE_GENERIC_RSP_T *opple_rsp = (OPPLE_GENERIC_RSP_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_CURTAIN_PERCENT_RSP;
    opple_msg->group_number = 0;
    opple_rsp->opcode       = OPPLE_SUCCESS;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_GENERIC_RSP_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_curtain_present_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_CURTAIN_PRESENT_PERCENT_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                          *sig_msg              = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                        *vendor_msg           = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                        *opple_msg            = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_CURTAIN_PRESENT_PERCENT_STATUS_T *sig_curtain_status   = (VENDOR_CURTAIN_PRESENT_PERCENT_STATUS_T *)(vendor_msg->data);
    OPPLE_CURTAIN_NOTIFY_PARA_T             *curtain_notify_para  = (OPPLE_CURTAIN_NOTIFY_PARA_T *)(opple_msg->para);

    opple_msg->state             = OPPLE_STATE_NORMAL;
    curtain_notify_para->percent = sig_curtain_status->percent;

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

/* PIR sensor */
static int vendor_2_opple_pir_sensor_report_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_PIR_SENSOR_EVENT_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_PIR_SENSOR_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                   *sig_msg          = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                 *vendor_msg       = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T                 *opple_msg        = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_PIR_SENSOR_EVENT_STATUS_T *sig_pir_status   = (VENDOR_PIR_SENSOR_EVENT_STATUS_T *)(vendor_msg->data);
    OPPLE_PIR_SENSOR_T               *opple_pir_status = (OPPLE_PIR_SENSOR_T *)(opple_msg->data);

    opple_msg->msg_id        = OPPLE_MSG_PIR_SENSOR_EVT_REPORT;
    opple_msg->group_number  = dnlink1_calc_gpno(sig_msg->src,OPP_SENSOR_GPNB_HASH_OBJ_ID);
    opple_pir_status->status = sig_pir_status->event;

    jx_hal_log_notice("opp pir evt:%d",opple_pir_status->status);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_PIR_SENSOR_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_pir_sensor_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_PIR_SENSOR_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                 *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T               *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY               *opple_msg       = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_PIR_SENSOR_STATUS_T     *sig_pir_status  = (VENDOR_PIR_SENSOR_STATUS_T *)(vendor_msg->data);
    OPPLE_PIR_SENSOR_NOTIFY_PARA_T *pir_notify_para = (OPPLE_PIR_SENSOR_NOTIFY_PARA_T *)(opple_msg->para);

    opple_msg->state        = OPPLE_STATE_NORMAL;
    pir_notify_para->status = sig_pir_status->status;

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_pir_sensor_battery_alarm_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_BATTERY_ALARM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                    *sig_msg              = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                  *vendor_msg           = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                  *opple_msg            = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_DEV_BATTERY_ALARM_STATUS_T *sig_bat_alarm_status = (VENDOR_DEV_BATTERY_ALARM_STATUS_T *)(vendor_msg->data);
    OPPLE_PIR_SENSOR_NOTIFY_PARA_T    *pir_notify_para      = (OPPLE_PIR_SENSOR_NOTIFY_PARA_T *)(opple_msg->para);

    opple_msg->state        = sig_bat_alarm_status->alarm ? OPPLE_STATE_LOW_POWER : OPPLE_STATE_NORMAL;
    pir_notify_para->status = OPPLE_SENSOR_STATUS_INVALID;

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

/* Door sensor */
static int vendor_2_opple_door_sensor_report_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DOOR_SENSOR_EVENT_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DOOR_SENSOR_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                    *sig_msg           = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                  *vendor_msg        = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T                  *opple_msg         = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_DOOR_SENSOR_EVENT_STATUS_T *sig_door_status   = (VENDOR_DOOR_SENSOR_EVENT_STATUS_T *)(vendor_msg->data);
    OPPLE_DOOR_SENSOR_T               *opple_door_status = (OPPLE_DOOR_SENSOR_T *)(opple_msg->data);

    opple_msg->msg_id         = OPPLE_MSG_DOOR_SENSOR_DATA_REPORT;
    opple_msg->group_number   = dnlink1_calc_gpno(sig_msg->src,OPP_SENSOR_GPNB_HASH_OBJ_ID);
    opple_door_status->status = sig_door_status->event;

    jx_hal_log_notice("opp door evt:%d",opple_door_status->status);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DOOR_SENSOR_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_door_sensor_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DOOR_SENSOR_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                  *sig_msg          = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                *vendor_msg       = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                *opple_msg        = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_DOOR_SENSOR_STATUS_T     *sig_door_status  = (VENDOR_DOOR_SENSOR_STATUS_T *)(vendor_msg->data);
    OPPLE_DOOR_SENSOR_NOTIFY_PARA_T *door_notify_para = (OPPLE_DOOR_SENSOR_NOTIFY_PARA_T *)(opple_msg->para);

    opple_msg->state         = OPPLE_STATE_NORMAL;
    door_notify_para->status = sig_door_status->status;

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_door_sensor_battery_alarm_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_BATTERY_ALARM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                    *sig_msg              = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                  *vendor_msg           = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                  *opple_msg            = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_DEV_BATTERY_ALARM_STATUS_T *sig_bat_alarm_status = (VENDOR_DEV_BATTERY_ALARM_STATUS_T *)(vendor_msg->data);
    OPPLE_DOOR_SENSOR_NOTIFY_PARA_T   *door_notify_para     = (OPPLE_DOOR_SENSOR_NOTIFY_PARA_T *)(opple_msg->para);

    opple_msg->state         = sig_bat_alarm_status->alarm ? OPPLE_STATE_LOW_POWER : OPPLE_STATE_NORMAL;
    door_notify_para->status = OPPLE_SENSOR_STATUS_INVALID;

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

/* Water sensor */
static int vendor_2_opple_water_sensor_report_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_WATER_SENSOR_ALARM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_WATER_SENSOR_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                     *sig_msg            = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                   *vendor_msg         = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T                   *opple_msg          = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_WATER_SENSOR_ALARM_STATUS_T *sig_water_status   = (VENDOR_WATER_SENSOR_ALARM_STATUS_T *)(vendor_msg->data);
    OPPLE_WATER_SENSOR_T               *opple_water_status = (OPPLE_WATER_SENSOR_T *)(opple_msg->data);

    opple_msg->msg_id          = OPPLE_MSG_WATER_SENSOR_DATA_REPORT;
    opple_msg->group_number    = dnlink1_calc_gpno(sig_msg->src,OPP_SENSOR_GPNB_HASH_OBJ_ID);
    opple_water_status->status = sig_water_status->alarm;

    jx_hal_log_notice("opp water evt:%d",opple_water_status->status);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_WATER_SENSOR_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_water_sensor_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_WATER_SENSOR_ALARM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                     *sig_msg           = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                   *vendor_msg        = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                   *opple_msg         = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_WATER_SENSOR_ALARM_STATUS_T *sig_water_status  = (VENDOR_WATER_SENSOR_ALARM_STATUS_T *)(vendor_msg->data);
    OPPLE_WATER_SENSOR_NOTIFY_PARA_T   *water_notify_para = (OPPLE_WATER_SENSOR_NOTIFY_PARA_T *)(opple_msg->para);

    opple_msg->state          = OPPLE_STATE_NORMAL;
    water_notify_para->status = sig_water_status->alarm;

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_water_sensor_battery_alarm_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_BATTERY_ALARM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                    *sig_msg              = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                  *vendor_msg           = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                  *opple_msg            = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_DEV_BATTERY_ALARM_STATUS_T *sig_bat_alarm_status = (VENDOR_DEV_BATTERY_ALARM_STATUS_T *)(vendor_msg->data);
    OPPLE_WATER_SENSOR_NOTIFY_PARA_T  *water_notify_para    = (OPPLE_WATER_SENSOR_NOTIFY_PARA_T *)(opple_msg->para);

    opple_msg->state          = sig_bat_alarm_status->alarm ? OPPLE_STATE_LOW_POWER : OPPLE_STATE_NORMAL;
    water_notify_para->status = OPPLE_SENSOR_STATUS_INVALID;

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

/* Temperature and humidity sensor */
static int vendor_2_opple_tem_hum_sensor_tem_report_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_TEM_HUM_SENSOR_TEM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_TEM_HUM_SENSOR_TEM_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                     *sig_msg          = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                   *vendor_msg       = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T                   *opple_msg        = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_TEM_HUM_SENSOR_TEM_STATUS_T *sig_tem_status   = (VENDOR_TEM_HUM_SENSOR_TEM_STATUS_T *)(vendor_msg->data);
    OPPLE_TEM_HUM_SENSOR_TEM_T         *opple_tem_status = (OPPLE_TEM_HUM_SENSOR_TEM_T *)(opple_msg->data);

    opple_msg->msg_id             = OPPLE_MSG_TMP_SENSOR_DATA_REPORT;
    opple_msg->group_number       = dnlink1_calc_gpno(sig_msg->src,OPP_SENSOR_GPNB_HASH_OBJ_ID);
    opple_tem_status->temperature = M_OPP_NTOHS(OPPLE_KELVIN_TO_CELSIUS(sig_tem_status->temperature));

    jx_hal_log_notice("sig tem：%d,opp tem: %d", sig_tem_status->temperature,opple_tem_status->temperature);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_TEM_HUM_SENSOR_TEM_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_tem_hum_sensor_tem_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_TEM_HUM_SENSOR_TEM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                      *sig_msg             = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                    *vendor_msg          = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                    *opple_msg           = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_TEM_HUM_SENSOR_TEM_STATUS_T  *sig_tem_status      = (VENDOR_TEM_HUM_SENSOR_TEM_STATUS_T *)(vendor_msg->data);
    OPPLE_TEM_HUM_SENSOR_NOTIFY_PARAM_T *tem_hum_notify_para = (OPPLE_TEM_HUM_SENSOR_NOTIFY_PARAM_T *)(opple_msg->para);

    opple_msg->state                 = OPPLE_STATE_NORMAL;
    tem_hum_notify_para->temperature = M_OPP_NTOHS(OPPLE_KELVIN_TO_CELSIUS(sig_tem_status->temperature));
    tem_hum_notify_para->humidity    = OPPLE_INVALID_VALUE;

    //jx_hal_log_notice("sig tem：%d,opp tem: %d", sig_tem_status->temperature,tem_hum_notify_para->temperature);

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_tem_hum_sensor_hum_report_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_TEM_HUM_SENSOR_HUM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_TEM_HUM_SENSOR_HUM_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                     *sig_msg          = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                   *vendor_msg       = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T                   *opple_msg        = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_TEM_HUM_SENSOR_HUM_STATUS_T *sig_hum_status   = (VENDOR_TEM_HUM_SENSOR_HUM_STATUS_T *)(vendor_msg->data);
    OPPLE_TEM_HUM_SENSOR_HUM_T         *opple_hum_status = (OPPLE_TEM_HUM_SENSOR_HUM_T *)(opple_msg->data);

    opple_msg->msg_id          = OPPLE_MSG_HDY_SENSOR_DATA_REPORT;
    opple_msg->group_number    = dnlink1_calc_gpno(sig_msg->src,OPP_SENSOR_GPNB_HASH_OBJ_ID);
    opple_hum_status->humidity = sig_hum_status->humidity / 10;

    jx_hal_log_notice("sig hum:%d,opp hum: %d", sig_hum_status->humidity,opple_hum_status->humidity);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_TEM_HUM_SENSOR_HUM_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_tem_hum_sensor_hum_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_TEM_HUM_SENSOR_HUM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                      *sig_msg             = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                    *vendor_msg          = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                    *opple_msg           = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_TEM_HUM_SENSOR_HUM_STATUS_T  *sig_hum_status      = (VENDOR_TEM_HUM_SENSOR_HUM_STATUS_T *)(vendor_msg->data);
    OPPLE_TEM_HUM_SENSOR_NOTIFY_PARAM_T *tem_hum_notify_para = (OPPLE_TEM_HUM_SENSOR_NOTIFY_PARAM_T *)(opple_msg->para);

    opple_msg->state                 = OPPLE_STATE_NORMAL;
    tem_hum_notify_para->temperature = OPPLE_INVALID_VALUE;
    tem_hum_notify_para->humidity    = sig_hum_status->humidity / 10;

    //jx_hal_log_notice("sig hum:%d,opp hum: %d", sig_hum_status->humidity,tem_hum_notify_para->humidity);

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_tem_hum_sensor_battery_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_BATTERY_LEVEL_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                      *sig_msg             = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                    *vendor_msg          = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                    *opple_msg           = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_DEV_BATTERY_LEVEL_STATUS_T   *sig_bat_level       = (VENDOR_DEV_BATTERY_LEVEL_STATUS_T *)(vendor_msg->data);
    OPPLE_TEM_HUM_SENSOR_NOTIFY_PARAM_T *tem_hum_bat_level   = (OPPLE_TEM_HUM_SENSOR_NOTIFY_PARAM_T *)(opple_msg->para);

    tem_hum_bat_level->battery_level = sig_bat_level->level;

    jx_hal_log_notice("opp tem bat:%d",tem_hum_bat_level->battery_level);

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_tem_hum_sensor_battery_alarm_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_BATTERY_ALARM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                     *sig_msg              = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                   *vendor_msg           = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                   *opple_msg            = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_DEV_BATTERY_ALARM_STATUS_T  *sig_bat_alarm_status = (VENDOR_DEV_BATTERY_ALARM_STATUS_T *)(vendor_msg->data);
    OPPLE_TEM_HUM_SENSOR_NOTIFY_PARAM_T *tem_hum_notify_para = (OPPLE_TEM_HUM_SENSOR_NOTIFY_PARAM_T *)(opple_msg->para);

    opple_msg->state                 = sig_bat_alarm_status->alarm ? OPPLE_STATE_LOW_POWER : OPPLE_STATE_NORMAL;
    tem_hum_notify_para->temperature = OPPLE_TEM_STATUS_INVALID;
    tem_hum_notify_para->humidity    = OPPLE_HUM_STATUS_INVALID;

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

/* Door lock */
static int vendor_2_opple_door_lock_alarm_report_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DOOR_LOCK_ALARM_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DOOR_LOCK_UNUSUAL_CODE_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                 *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T               *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T               *opple_msg       = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_DOOR_LOCK_ALARM_T       *sig_door_lock   = (VENDOR_DOOR_LOCK_ALARM_T *)(vendor_msg->data);
    OPPLE_DOOR_LOCK_UNUSUAL_CODE_T *opple_door_lock = (OPPLE_DOOR_LOCK_UNUSUAL_CODE_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_DOOR_LOCK_UNUSUAL_REPORT;
    opple_msg->group_number = dnlink1_calc_gpno(sig_msg->src,OPP_SENSOR_GPNB_HASH_OBJ_ID);
    opple_door_lock->code   = sig_door_lock->alarm;

    jx_hal_log_notice("door lock alarm: %d",opple_door_lock->code);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DOOR_LOCK_UNUSUAL_CODE_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_door_unlock_report_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DOOR_UNLOCK_EVENT_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DOOR_LOCK_UNLOCK_EVENT_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T              *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T              *opple_msg       = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_DOOR_UNLOCK_EVENT_T    *sig_door_lock   = (VENDOR_DOOR_UNLOCK_EVENT_T *)(vendor_msg->data);
    OPPLE_DOOR_LOCK_UNLOCK_EVENT_T  *opple_door_lock = (OPPLE_DOOR_LOCK_UNLOCK_EVENT_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_DOOR_UNLOCK_REPORT;
    opple_msg->group_number = dnlink1_calc_gpno(sig_msg->src,OPP_SENSOR_GPNB_HASH_OBJ_ID);
    opple_door_lock->id     = sig_door_lock->id;
    opple_door_lock->type   = sig_door_lock->type;

    jx_hal_log_notice("door unlock type: %d", opple_door_lock->type);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DOOR_LOCK_UNLOCK_EVENT_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_door_lock_on_report_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DOOR_LOCK_ON_EVENT_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DOOR_LOCK_ON_EVENT_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T              *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T            *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T            *opple_msg       = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_DOOR_LOCK_ON_EVENT_T *sig_door_lock   = (VENDOR_DOOR_LOCK_ON_EVENT_T *)(vendor_msg->data);
    OPPLE_DOOR_LOCK_ON_EVENT_T  *opple_door_lock = (OPPLE_DOOR_LOCK_ON_EVENT_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_DOOR_LOCK_ON_REPORT;
    opple_msg->group_number = dnlink1_calc_gpno(sig_msg->src,OPP_SENSOR_GPNB_HASH_OBJ_ID);
    opple_door_lock->event  = sig_door_lock->event;

    jx_hal_log_notice("door lock on: %d", opple_door_lock->event);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DOOR_LOCK_ON_EVENT_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_door_lock_event_report_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DOOR_LOCK_EVENT_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DOOR_LOCK_EVENT_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T           *sig_msg           = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T         *vendor_msg        = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T         *opple_msg         = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_DOOR_LOCK_EVENT_T *sig_door_lock   = (VENDOR_DOOR_LOCK_EVENT_T *)(vendor_msg->data);
    OPPLE_DOOR_LOCK_EVENT_T  *opple_door_lock = (OPPLE_DOOR_LOCK_EVENT_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_DOOR_UNLOCK_REPORT;
    opple_msg->group_number = dnlink1_calc_gpno(sig_msg->src,OPP_SENSOR_GPNB_HASH_OBJ_ID);
    opple_door_lock->event  = 0x01;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DOOR_LOCK_EVENT_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_door_lock_open_mode_report_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DOOR_LOCK_OPEN_MODE_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DOOR_LOCK_OPEN_MODE_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T               *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T             *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T             *opple_msg       = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_DOOR_LOCK_OPEN_MODE_T *sig_door_lock   = (VENDOR_DOOR_LOCK_OPEN_MODE_T *)(vendor_msg->data);
    OPPLE_DOOR_LOCK_OPEN_MODE_T  *opple_door_lock = (OPPLE_DOOR_LOCK_OPEN_MODE_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_DOOR_LOCK_EVENT_REPORT;
    opple_msg->group_number = dnlink1_calc_gpno(sig_msg->src,OPP_SENSOR_GPNB_HASH_OBJ_ID);
    opple_door_lock->id     = (uint16_t)OPPLE_INVALID_VALUE;

    opple_door_lock->mode = sig_door_lock->mode;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DOOR_LOCK_OPEN_MODE_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_door_lock_user_add_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DOOR_LOCK_USER_ADD_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DOOR_LOCK_USER_ADD_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T              *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T            *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T            *opple_msg       = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_DOOR_LOCK_USER_ADD_T *sig_door_lock   = (VENDOR_DOOR_LOCK_USER_ADD_T *)(vendor_msg->data);
    OPPLE_DOOR_LOCK_USER_ADD_T  *opple_door_lock = (OPPLE_DOOR_LOCK_USER_ADD_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_DOOR_LOCK_USER_ADD;
    opple_msg->group_number = 0;
    opple_door_lock->user_id   = sig_door_lock->user_id;
    opple_door_lock->lock_type = sig_door_lock->lock_type;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DOOR_LOCK_USER_ADD_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_door_lock_user_del_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DOOR_LOCK_USER_DEL_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DOOR_LOCK_USER_DEL_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T              *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T            *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T            *opple_msg       = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_DOOR_LOCK_USER_DEL_T *sig_door_lock   = (VENDOR_DOOR_LOCK_USER_DEL_T *)(vendor_msg->data);
    OPPLE_DOOR_LOCK_USER_DEL_T  *opple_door_lock = (OPPLE_DOOR_LOCK_USER_DEL_T *)(opple_msg->data);

    opple_msg->msg_id          = OPPLE_MSG_DOOR_LOCK_USER_DEL;
    opple_msg->group_number    = 0;
    opple_door_lock->user_id   = sig_door_lock->user_id;
    opple_door_lock->lock_type = sig_door_lock->lock_type;

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DOOR_LOCK_USER_DEL_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_door_lock_pwd_add_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DOOR_LOCK_PWD_ADD_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DOOR_LOCK_PWD_ADD_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T              *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T            *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T            *opple_msg       = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_DOOR_LOCK_PWD_ADD_T *sig_door_lock   = (VENDOR_DOOR_LOCK_PWD_ADD_T *)(vendor_msg->data);
    OPPLE_DOOR_LOCK_PWD_ADD_T  *opple_door_lock = (OPPLE_DOOR_LOCK_PWD_ADD_T *)(opple_msg->data);

    opple_msg->msg_id          = OPPLE_MSG_DOOR_LOCK_PWD_ADD;
    opple_msg->group_number    = 0;
    
    opple_door_lock->len = sig_door_lock->len;
    OPPLE_MEMCPY((uint8_t *)opple_door_lock->password,(uint8_t *)sig_door_lock->password,opple_door_lock->len);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DOOR_LOCK_PWD_ADD_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_door_lock_battery_alarm_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_BATTERY_ALARM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DOOR_LOCK_UNUSUAL_CODE_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                    *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                  *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T                  *opple_msg       = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_DEV_BATTERY_ALARM_STATUS_T *sig_door_lock   = (VENDOR_DEV_BATTERY_ALARM_STATUS_T *)(vendor_msg->data);
    OPPLE_DOOR_LOCK_UNUSUAL_CODE_T    *opple_door_lock = (OPPLE_DOOR_LOCK_UNUSUAL_CODE_T *)(opple_msg->data);

    opple_msg->msg_id          = OPPLE_MSG_DOOR_LOCK_UNUSUAL_REPORT;
    opple_msg->group_number    = dnlink1_calc_gpno(sig_msg->src,OPP_SENSOR_GPNB_HASH_OBJ_ID);
    
    if(sig_door_lock->alarm){
        opple_door_lock->code = OPPLE_DOOR_LOCK_BATTERY_ALARM;
    }else{
        opple_door_lock->code = OPPLE_DOOR_LOCK_BATTERY_ALARM_CLEAR;
    }

    jx_hal_log_notice("door lock bat alarm: %d",opple_door_lock->code);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DOOR_LOCK_UNUSUAL_CODE_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_door_lock_battery_alarm_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_BATTERY_ALARM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                     *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                   *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                   *opple_msg       = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_DEV_BATTERY_ALARM_STATUS_T  *sig_door_lock   = (VENDOR_DEV_BATTERY_ALARM_STATUS_T *)(vendor_msg->data);
    OPPLE_DOOR_LOCK_NOTIFY_PARAM_T     *opple_door_lock = (OPPLE_DOOR_LOCK_NOTIFY_PARAM_T *)(opple_msg->para);

    opple_msg->state               = sig_door_lock->alarm ? OPPLE_STATE_LOW_POWER : OPPLE_STATE_NORMAL;
    opple_door_lock->locked        = (uint8_t)OPPLE_INVALID_VALUE;
    opple_door_lock->battery_level = (uint8_t)OPPLE_INVALID_VALUE;

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_door_lock_battery_level_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_BATTERY_LEVEL_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                     *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                   *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                   *opple_msg       = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_DEV_BATTERY_LEVEL_STATUS_T  *sig_door_lock   = (VENDOR_DEV_BATTERY_LEVEL_STATUS_T *)(vendor_msg->data);
    OPPLE_DOOR_LOCK_NOTIFY_PARAM_T     *opple_door_lock = (OPPLE_DOOR_LOCK_NOTIFY_PARAM_T *)(opple_msg->para);

    opple_msg->state               = (uint8_t)OPPLE_INVALID_VALUE;;
    opple_door_lock->locked        = (uint8_t)OPPLE_INVALID_VALUE;
    opple_door_lock->battery_level = sig_door_lock->level;

    jx_hal_log_notice("door lock bat level: %d",opple_door_lock->battery_level);

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_door_lock_dismantle_alarm_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_SENSOR_DISMANTLE_ALARM_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DOOR_LOCK_UNUSUAL_CODE_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                  *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T                *opple_msg       = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_SENSOR_DISMANTLE_ALARM_T *sig_door_lock   = (VENDOR_SENSOR_DISMANTLE_ALARM_T *)(vendor_msg->data);
    OPPLE_DOOR_LOCK_UNUSUAL_CODE_T  *opple_door_lock = (OPPLE_DOOR_LOCK_UNUSUAL_CODE_T *)(opple_msg->data);

    opple_msg->msg_id          = OPPLE_MSG_DOOR_LOCK_UNUSUAL_REPORT;
    opple_msg->group_number    = dnlink1_calc_gpno(sig_msg->src,OPP_SENSOR_GPNB_HASH_OBJ_ID);
    
    if(sig_door_lock->alarm){
        opple_door_lock->code = OPPLE_DOOR_LOCK_TAMPER_ALARM;
    }else{
        opple_door_lock->code = OPPLE_DOOR_LOCK_TAMPER_ALARM_CLEAR;
    }

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DOOR_LOCK_UNUSUAL_CODE_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_door_lock_status_report_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DOOR_LOCK_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                 *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T               *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY               *opple_msg       = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_DOOR_LOCK_STATUS_T      *sig_door_lock   = (VENDOR_DOOR_LOCK_STATUS_T *)(vendor_msg->data);
    OPPLE_DOOR_LOCK_NOTIFY_PARAM_T *opple_door_lock = (OPPLE_DOOR_LOCK_NOTIFY_PARAM_T *)(opple_msg->para);

    opple_msg->state               = sig_door_lock->status;
    opple_door_lock->locked        = (uint8_t)OPPLE_INVALID_VALUE;
    opple_door_lock->battery_level = (uint8_t)OPPLE_INVALID_VALUE;

    jx_hal_log_notice("door lock state: %d",opple_msg->state);

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_low_power_sensor_battery_alarm_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_BATTERY_ALARM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                     *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                   *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                   *opple_msg       = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_DEV_BATTERY_ALARM_STATUS_T  *sig_bat_alarm   = (VENDOR_DEV_BATTERY_ALARM_STATUS_T *)(vendor_msg->data);

    opple_msg->state = sig_bat_alarm->alarm ? OPPLE_STATE_LOW_POWER : OPPLE_STATE_NORMAL;

    jx_hal_log_notice("opp bat alarm:%d",sig_bat_alarm->alarm);

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_low_power_sensor_battery_level_status_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_BATTERY_LEVEL_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + SIG_MESH_SINGLE_PKT_SIZE ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                     *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                   *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_DEV_NOTIFY                   *opple_msg       = (OPPLE_DEV_NOTIFY *)out->data;
    VENDOR_DEV_BATTERY_LEVEL_STATUS_T  *sig_bat_level   = (VENDOR_DEV_BATTERY_LEVEL_STATUS_T *)(vendor_msg->data);
    OPPLE_LP_SENSOR_NOTIFY_PARAM_T     *opple_bat_level = (OPPLE_LP_SENSOR_NOTIFY_PARAM_T *)(opple_msg->para);

    opple_bat_level->battery_level = sig_bat_level->level;

    jx_hal_log_notice("opp bat level:%d",opple_bat_level->battery_level);

    out->data_len  = SIG_MESH_SINGLE_PKT_SIZE;
    out->pack_type = OPPLE_STATUS;
    out->addr      = sig_msg->src;
    return 0;
}

/* Door bell */
static int vendor_2_opple_door_bell_alarm_report_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DOOR_BELL_ALARM_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DOOR_BELL_EVENT_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T            *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T          *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T          *opple_msg       = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_DOOR_BELL_ALARM_T  *sig_door_bell   = (VENDOR_DOOR_BELL_ALARM_T *)(vendor_msg->data);
    OPPLE_DOOR_BELL_EVENT_T   *opple_door_bell = (OPPLE_DOOR_BELL_EVENT_T *)(opple_msg->data);

    opple_msg->msg_id       = OPPLE_MSG_DOOR_BELL_REPORT;
    opple_msg->group_number = dnlink1_calc_gpno(sig_msg->src,OPP_SENSOR_GPNB_HASH_OBJ_ID);

    opple_door_bell->event = sig_door_bell->alarm;

    jx_hal_log_notice("door bell alarm: %d",opple_door_bell->event);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DOOR_BELL_EVENT_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

static int vendor_2_opple_low_power_dev_battery_alarm_report_func(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len)
{
    if (in == NULL || in->data == NULL ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(VENDOR_DEV_BATTERY_ALARM_STATUS_T) ||
        buf_len < sizeof(OPPLE_OUT_T) + sizeof(OPPLE_MESH_MSG_T) + sizeof(OPPLE_DEV_BATTERY_ALARM_T) ||
        out == NULL || out->data == NULL) return -1;

    SIG_MESH_RSP_T                    *sig_msg         = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T                  *vendor_msg      = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPPLE_MESH_MSG_T                  *opple_msg       = (OPPLE_MESH_MSG_T *)out->data;
    VENDOR_DEV_BATTERY_ALARM_STATUS_T *sig_door_lock   = (VENDOR_DEV_BATTERY_ALARM_STATUS_T *)(vendor_msg->data);
    OPPLE_DEV_BATTERY_ALARM_T         *opple_door_lock = (OPPLE_DEV_BATTERY_ALARM_T *)(opple_msg->data);

    opple_msg->msg_id          = OPPLE_MSG_BATTERY_ALARM_REPORT;
    opple_msg->group_number    = dnlink1_calc_gpno(sig_msg->src,OPP_SENSOR_GPNB_HASH_OBJ_ID);
    
    opple_door_lock->alarm = sig_door_lock->alarm;
    jx_hal_log_notice("bat alarm report:%d",opple_door_lock->alarm);

    out->data_len  = MSG_HEAD_SIZE + sizeof(OPPLE_DEV_BATTERY_ALARM_T);
    out->pack_type = OPPLE_EVENT;
    out->addr      = sig_msg->src;
    return 0;
}

const OPP_SIG_MAPPINT_T g_protocol_mapping[] = {
    {OPPLE_MSG_ONOFF,                     G_ONOFF_SET,                            sig_2_opple_onoff_set_func},
    {OPPLE_MSG_ONOFF_RSP,                 G_ONOFF_STATUS,                         sig_2_opple_onoff_set_rsp_func},
    //{OPPLE_MSG_NOTIFY_RSP,                G_ONOFF_STATUS,                         sig_2_opple_onoff_status_func},

    {OPPLE_MSG_MODIFY_BRIGHT,             LIGHTNESS_SET,                          sig_2_opple_lightness_set_func},
    {OPPLE_MSG_MODIFY_BRIGHT_RSP,         LIGHTNESS_STATUS,                       sig_2_opple_lightness_set_rsp_func},
    //{OPPLE_MSG_NOTIFY_RSP,                LIGHTNESS_STATUS,                       sig_2_opple_lightness_status_func},

    {OPPLE_MSG_MODIFY_CCT,                LIGHT_CTL_TEMP_SET,                     sig_2_opple_light_cct_set_func},
    {OPPLE_MSG_MODIFY_CCT_RSP,            LIGHT_CTL_TEMP_STATUS,                  sig_2_opple_light_cct_set_rsp_func},
    //{OPPLE_MSG_NOTIFY_RSP,                LIGHT_CTL_TEMP_STATUS,                  sig_2_opple_light_cct_status_func},
    {OPPLE_MSG_NOTIFY_RSP,                LIGHT_CTL_STATUS,                       sig_2_opple_lightness_cct_status_func},

    {OPPLE_MSG_ADD_GRP,                   CFG_MODEL_SUB_ADD,                      sig_2_opple_group_add_func},
    {OPPLE_MSG_DEL_GRP,                   CFG_MODEL_SUB_DEL,                      sig_2_opple_group_delete_func},
    {OPPLE_DEV_CONFIG_SETTING_RSP,        CFG_MODEL_SUB_STATUS,                   sig_2_opple_group_config_set_rsp_func},

    //{OPPLE_MSG_ADD_SENCE,                 SCENE_STORE,                            sig_2_opple_scene_store_func},
    //{OPPLE_MSG_ADD_SENCE_RSP,             SCENE_REG_STAUTS,                       sig_2_opple_scene_store_rsp_func},
    //{OPPLE_MSG_CALL_SENCE,                SCENE_CALL,                             sig_2_opple_scene_recall_func},
    //{OPPLE_MSG_CALL_SENCE_RSP,            SCENE_STATUS,                           sig_2_opple_scene_recall_rsp_func},
    //{OPPLE_MSG_DEL_SENCE,                 SCENE_DELETE,                           sig_2_opple_scene_delete_func},
    //{OPPLE_MSG_DEL_SENCE_RSP,             SCENE_REG_STAUTS,                       sig_2_opple_scene_delete_rsp_func},
    {OPPLE_DEV_CONFIG_SETTING_RSP,        SCENE_REG_STAUTS,                       sig_2_opple_scene_config_set_rsp_func},

    {OPPLE_MSG_FACTORY_RESET,             NODE_RESET,                             sig_2_opple_dev_reset_func},
    {OPPLE_MSG_FACTORY_RESET_RSP,         NODE_RESET_STATUS,                      sig_2_opple_dev_reset_rsp_func},

    //{OPPLE_MSG_DOOR_LOCK_OPEN,            G_ONOFF_SET,                          sig_2_opple_door_lock_open_func},

    /* Vendor to Opple */
    //{OPPLE_MSG_NOTIFY_RSP,              VENDOR_ATTR_ERROR_CODE,                 vendor_2_opple_device_status_func},
    {OPPLE_MSG_DEV_UPGRADE_IND,           VENDOR_ATTR_VER_STATUS,                 vendor_2_opple_device_version_func},
    {OPPLE_MSG_SYNC_TIME,                 VENDOR_ATTR_TIME_ZONE,                  vendor_2_opple_device_time_zone_func},
    {OPPLE_MSG_SYNC_TIME,                 VENDOR_ATTR_UNIX_TIME,                  vendor_2_opple_device_unix_time_func},

    {OPPLE_MSG_SCENE_PANEL_KEY_REPORT,    VENDOR_ATTR_SCENE_PANEL_KEY_NUM,        vendor_2_opple_scene_panel_key_report_func},  
    {OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_BATTERY_LEVEL,              vendor_2_opple_scene_panel_battery_status_func},
    //{OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_BATTERY_ALARM,              vendor_2_opple_scene_panel_battery_alarm_status_func},

    {OPPLE_MSG_CURTAIN_PERCENT_RSP,       VENDOR_ATTR_CURTAIN_TARGET_PERCENT,     vendor_2_opple_curtain_present_set_rsp_func},
    {OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_CURTAIN_PRESENT_PERCENT,    vendor_2_opple_curtain_present_status_func},
    {OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_CURTAIN_TARGET_PERCENT,     vendor_2_opple_curtain_target_status_func},

    {OPPLE_MSG_PIR_SENSOR_EVT_REPORT,     VENDOR_ATTR_PIR_SENSOR_EVENT,           vendor_2_opple_pir_sensor_report_func},
    {OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_PIR_SENSOR_STATUS,          vendor_2_opple_pir_sensor_status_func},
    //{OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_BATTERY_ALARM,              vendor_2_opple_pir_sensor_battery_alarm_status_func},

    {OPPLE_MSG_DOOR_SENSOR_DATA_REPORT,   VENDOR_ATTR_DOOR_SENSOR_EVENT,          vendor_2_opple_door_sensor_report_func},
    {OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_DOOR_SENSOR_EVENT,          vendor_2_opple_door_sensor_status_func},
    {OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_DOOR_SENSOR_STATUS,         vendor_2_opple_door_sensor_status_func},
    //{OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_BATTERY_ALARM,              vendor_2_opple_door_sensor_battery_alarm_status_func},

    {OPPLE_MSG_WATER_SENSOR_DATA_REPORT,  VENDOR_ATTR_SENSOR_ALARM,               vendor_2_opple_water_sensor_report_func},
    {OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_SENSOR_ALARM,               vendor_2_opple_water_sensor_status_func},
    //{OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_BATTERY_ALARM,              vendor_2_opple_water_sensor_battery_alarm_status_func},

    {OPPLE_MSG_TMP_SENSOR_DATA_REPORT,    VENDOR_ATTR_TEM_HUM_SENSOR_TEM,         vendor_2_opple_tem_hum_sensor_tem_report_func},
    {OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_TEM_HUM_SENSOR_TEM,         vendor_2_opple_tem_hum_sensor_tem_status_func},
    {OPPLE_MSG_HDY_SENSOR_DATA_REPORT,    VENDOR_ATTR_TEM_HUM_SENSOR_HUM,         vendor_2_opple_tem_hum_sensor_hum_report_func},
    {OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_TEM_HUM_SENSOR_HUM,         vendor_2_opple_tem_hum_sensor_hum_status_func},
    {OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_BATTERY_LEVEL,              vendor_2_opple_tem_hum_sensor_battery_status_func},
    //{OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_BATTERY_ALARM,              vendor_2_opple_tem_hum_sensor_battery_alarm_status_func},

    {OPPLE_MSG_DOOR_LOCK_UNUSUAL_REPORT,  VENDOR_ATTR_DOOR_LOCK_ALARM_EVENT,      vendor_2_opple_door_lock_alarm_report_func},
    {OPPLE_MSG_DOOR_LOCK_ON_REPORT,       VENDOR_ATTR_DOOR_LOCK_ON_EVENT,         vendor_2_opple_door_lock_on_report_func},
    {OPPLE_MSG_DOOR_UNLOCK_REPORT,        VENDOR_ATTR_DOOR_LOCK_OPEN_EVENT,       vendor_2_opple_door_unlock_report_func},
    {OPPLE_MSG_DOOR_BELL_REPORT,          VENDOR_ATTR_DOOR_BELL_ALARM_EVENT,      vendor_2_opple_door_bell_alarm_report_func},
    {OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_DOOR_LOCK_STATUS,           vendor_2_opple_door_lock_status_report_func},
    {OPPLE_MSG_DOOR_LOCK_UNUSUAL_REPORT,  VENDOR_ATTR_SENSOR_DISMANTLE_ALARM,     vendor_2_opple_door_lock_dismantle_alarm_func},
    {OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_BATTERY_LEVEL,              vendor_2_opple_door_lock_battery_level_status_func},
    {OPPLE_MSG_DOOR_LOCK_UNUSUAL_REPORT,  VENDOR_ATTR_BATTERY_ALARM,              vendor_2_opple_door_lock_battery_alarm_func},
    //{OPPLE_MSG_DOOR_UNLOCK_REPORT,      VENDOR_ATTR_DOOR_LOCK_EVENT,            vendor_2_opple_door_lock_event_report_func},
    //{OPPLE_MSG_DOOR_UNLOCK_REPORT,      VENDOR_ATTR_DOOR_LOCK_OPEN_MODE,        vendor_2_opple_door_lock_open_mode_report_func},
    //{OPPLE_MSG_DOOR_LOCK_USER_ADD,      VENDOR_ATTR_DOOR_LOCK_USER_ADD,         vendor_2_opple_door_lock_user_add_func},
    //{OPPLE_MSG_DOOR_LOCK_USER_DEL,      VENDOR_ATTR_DOOR_LOCK_USER_DEL,         vendor_2_opple_door_lock_user_del_func},
    //{OPPLE_MSG_DOOR_LOCK_PWD_ADD,       VENDOR_ATTR_DOOR_LOCK_PWD_ADD,          vendor_2_opple_door_lock_pwd_add_func},
    //{OPPLE_MSG_NOTIFY_RSP,              VENDOR_ATTR_BATTERY_ALARM,              vendor_2_opple_door_lock_battery_alarm_status_func},
    
    {OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_BATTERY_LEVEL,              vendor_2_opple_low_power_sensor_battery_level_status_func},
    {OPPLE_MSG_NOTIFY_RSP,                VENDOR_ATTR_BATTERY_ALARM,              vendor_2_opple_low_power_sensor_battery_alarm_status_func},
    {OPPLE_MSG_BATTERY_ALARM_REPORT,      VENDOR_ATTR_BATTERY_ALARM,              vendor_2_opple_low_power_dev_battery_alarm_report_func},
    {NULL,NULL,NULL}
};

int sig_2_opple_protocol_convert(SIG_IN_T *in, void *out, uint8_t buf_len, int index)
{
    if (in == NULL || out == NULL || buf_len < sizeof(OPPLE_OUT_T) ||
        in->data_len < sizeof(SIG_MESH_RSP_T) + sizeof(uint16_t) ||
        index >= (sizeof(g_protocol_mapping) / sizeof(OPP_SIG_MAPPINT_T))) return -1;

    SIG_MESH_RSP_T    *sig_msg    = (SIG_MESH_RSP_T *)in->data;
    SIG_VENDOR_MSG_T  *vendor_msg = (SIG_VENDOR_MSG_T *)sig_msg->data;
    OPP_SIG_MAPPINT_T *mappint    = g_protocol_mapping;
    uint16_t           msg_id     = *(uint16_t *)sig_msg->data;

    for(uint8_t i = 0; ; i ++){
        if(msg_id == mappint[i].sigmesh_msg_id){
            break;
        }
        if(mappint[i].sigmesh_to_opple == NULL){
            msg_id = *(uint16_t *)vendor_msg->data;
            break;
        }
    }

    printf("msg id:%04X\n",msg_id);

    for(; mappint[index].sigmesh_to_opple != NULL; index ++){
        if(msg_id == mappint[index].sigmesh_msg_id){
            if(mappint[index].sigmesh_to_opple(in,(OPPLE_OUT_T *)out,buf_len) == 0){
                return (index + 1);
            }
        }
    }

    /* 解析电量&电量告警的黏合消息，当判断黏合时，第一个消息解析成功时则将另外一个消息进行移动覆盖，然后重新解析 */
    if(msg_id == VENDOR_ATTR_BATTERY_LEVEL || msg_id == VENDOR_ATTR_BATTERY_ALARM){
        /* VENDOR_DEV_BATTERY_ALARM_STATUS_T 和 VENDOR_DEV_BATTERY_LEVEL_STATUS_T 大小均为 3 个字节 */
        uint16_t msg_id_2 = *(uint16_t *)(vendor_msg->data + 0x03);

        if(msg_id_2 == VENDOR_ATTR_BATTERY_LEVEL || msg_id_2 == VENDOR_ATTR_BATTERY_ALARM){
            
            if(msg_id != msg_id_2){
                memcpy((void *)vendor_msg->data,(void *)(vendor_msg->data + 0x03),0x03);
                return 0;
            }
        }
    }

    return -1;
}
#ifdef __cplusplus
}
#endif

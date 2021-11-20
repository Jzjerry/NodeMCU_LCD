#include "display.h"

static lv_obj_t * label_UID;
static lv_obj_t * label_Followers;
static lv_obj_t * label_Onstream;
static lv_obj_t * circle_Onstream;
static lv_obj_t * circle_bg;


static void anim_size_cb(void * var, int32_t v)
{
    lv_obj_set_size((lv_obj_t*)var, v, v);
}

static void anim_opa_cb(void * var, int32_t v)
{
    lv_obj_set_style_opa((lv_obj_t*)var, v, 0);
}

void display_init()
{


    lv_obj_clean(lv_scr_act());

    circle_Onstream = lv_obj_create(lv_scr_act());
    label_UID = lv_label_create(lv_scr_act());
    label_Followers = lv_label_create(lv_scr_act());
    label_Onstream = lv_label_create(lv_scr_act());

    lv_obj_align(label_UID, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_align(label_Followers, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_align(label_Onstream, LV_ALIGN_CENTER, 0, 0);

    lv_obj_align(circle_Onstream, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_radius(circle_Onstream, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_opa(circle_Onstream, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(circle_Onstream, 5, 0);
    lv_obj_set_style_border_color(circle_Onstream, lv_palette_main(LV_PALETTE_LIGHT_GREEN), 0);
    // lv_obj_set_style_bg_color(circle_Onstream, lv_palette_main(LV_PALETTE_LIGHT_GREEN), 0);

}


void display_update(Streamer *Streamer_ptr)
{
    lv_label_set_text(label_UID, ("UID: " + Streamer_ptr->getUID()).c_str());
    lv_label_set_text_fmt(label_Followers, "Followers: %d", Streamer_ptr->getFollower());
    if(Streamer_ptr->getLiveStatus() == LIVE_ONSTREAM)
    {
        lv_label_set_text(label_Onstream, "Onstream");
        lv_anim_t anim_Onstream;
        lv_anim_init(&anim_Onstream);
        lv_anim_set_var(&anim_Onstream, circle_Onstream);
        lv_anim_set_values(&anim_Onstream, 0, 120);
        lv_anim_set_time(&anim_Onstream, 1000);
        lv_anim_set_path_cb(&anim_Onstream, lv_anim_path_linear);
        lv_anim_set_exec_cb(&anim_Onstream, anim_size_cb);
        lv_anim_start(&anim_Onstream);
    
        lv_anim_set_exec_cb(&anim_Onstream, anim_opa_cb);
        lv_anim_set_values(&anim_Onstream, 255, 0);
        lv_anim_start(&anim_Onstream);
    }
    else
    {
        lv_label_set_text(label_Onstream, "Offstream");
        lv_obj_set_style_opa(circle_Onstream, LV_OPA_0, 0);
    }
}
#include "display.h"

static lv_obj_t * label_UID;
static lv_obj_t * label_Followers;
static lv_obj_t * label_Onstream;
static lv_obj_t * circle_Onstream;
static lv_obj_t * label_Warning;
static lv_obj_t * label_WifiConnection;


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
    label_Onstream = lv_label_create(lv_scr_act());
    label_UID = lv_label_create(lv_scr_act());
    label_Followers = lv_label_create(lv_scr_act());

    label_Warning = lv_label_create(lv_scr_act());
    label_WifiConnection = lv_label_create(lv_scr_act());


    lv_obj_align(label_UID, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_align(label_Followers, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_align(label_Onstream, LV_ALIGN_CENTER, 0, 0);

    lv_obj_align(label_Warning, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_align(label_WifiConnection, LV_ALIGN_TOP_RIGHT, -10, 10);

    lv_obj_set_style_text_align(label_Followers, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_align(label_Onstream, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_align(label_UID, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_align(circle_Onstream, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_radius(circle_Onstream, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_opa(circle_Onstream, LV_OPA_COVER, 0);

    lv_label_set_text(label_Warning, LV_SYMBOL_WARNING);
    lv_label_set_text(label_WifiConnection, LV_SYMBOL_WIFI);
}


void display_update(Streamer *Streamer_ptr)
{
    lv_label_set_text(label_UID, ("UID:\n" + Streamer_ptr->getUID()).c_str());
    lv_label_set_text_fmt(label_Followers, "Followers:\n%d", Streamer_ptr->getFollower());

    lv_obj_set_style_text_color(label_Warning, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_text_color(label_WifiConnection, lv_palette_main(LV_PALETTE_GREEN), 0);

    if(Streamer_ptr->getLiveStatus() == LIVE_ONSTREAM)
    {
        lv_label_set_text(label_Onstream, LV_SYMBOL_PLAY "\nOnstream" );
        lv_obj_set_style_bg_color(circle_Onstream, lv_palette_main(LV_PALETTE_GREEN), 0);

        // Circle Animation
        lv_anim_t anim_Onstream;
        lv_anim_init(&anim_Onstream);
        lv_anim_set_var(&anim_Onstream, circle_Onstream);
        lv_anim_set_values(&anim_Onstream, 110, 130);
        lv_anim_set_time(&anim_Onstream, 750);
        lv_anim_set_playback_delay(&anim_Onstream, 0);
        lv_anim_set_playback_time(&anim_Onstream, 750);
        lv_anim_set_path_cb(&anim_Onstream, lv_anim_path_ease_in_out);
        lv_anim_set_exec_cb(&anim_Onstream, anim_size_cb);
        lv_anim_start(&anim_Onstream);
    
        lv_anim_set_exec_cb(&anim_Onstream, anim_opa_cb);
        lv_anim_set_values(&anim_Onstream, 255, 200);
        lv_anim_start(&anim_Onstream);
    }
    else
    {
        lv_label_set_text(label_Onstream, LV_SYMBOL_STOP "\nOffstream" );
        lv_obj_set_style_bg_color(circle_Onstream, lv_palette_main(LV_PALETTE_GREY), 0);
        lv_obj_set_style_opa(circle_Onstream, LV_OPA_50, 0);
        lv_obj_set_style_size(circle_Onstream, 110, 0);
    }
}


void display_error(display_error_t err)
{
    switch(err)
    {
        case DISPLAY_SIMP_FAIL:
            lv_obj_set_style_text_color(label_Warning, lv_palette_main(LV_PALETTE_RED), 0);
            break;
        case DISPLAY_STREAMER_FAIL:
            lv_obj_set_style_text_color(label_Warning, lv_palette_main(LV_PALETTE_YELLOW), 0);
            break;
        default:
            lv_obj_set_style_text_color(label_Warning, lv_palette_main(LV_PALETTE_GREY), 0);
            break;
    }

    if(WiFi.status() != WL_CONNECTED)
    {
        lv_obj_set_style_text_color(label_WifiConnection, lv_palette_main(LV_PALETTE_GREY), 0);
    }
}
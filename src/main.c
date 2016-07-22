#include <pebble.h>
#include "main.h"

//Window pointer
Window *window;

//Load Bitmap (Mystic Logo)
static GBitmap *bitmap_image;
static BitmapLayer *background_bitmap_layer;

//Make text layer for time
static TextLayer *time_display_layer;


static void update_time(){
  //get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //Write current hours/mins to buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H: %M" : "%I:%M", tick_time);
  text_layer_set_text(time_display_layer, s_buffer);
}

//Time running event
static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  update_time();
  
}


//Function that returns nothing and occurs on window load
void window_load(Window *window){
  
//BITMAP STUFF
  //Set bitmap_image to logo of the best Pokemon GO team.
  bitmap_image = gbitmap_create_with_resource(RESOURCE_ID_MYSTIC_IMAGE);
  
  //Create and set settings of a layer for the bitmap at 0,0 with a width of 144 and a height of 168
  background_bitmap_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_compositing_mode(background_bitmap_layer, GCompOpSet);
  bitmap_layer_set_bitmap(background_bitmap_layer, bitmap_image);
  
  //Add layer as child to window
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(background_bitmap_layer));

//TIME STUFF
  

 
  GRect bounds = layer_get_bounds(window_get_root_layer(window));
  
  //Create textlayer at 0,140 with the same width as the screen, and with a height of 28
  time_display_layer = text_layer_create(GRect(0, 140, bounds.size.w, 28));
  
  //Set layer styles
  text_layer_set_background_color(time_display_layer, GColorBlueMoon);
  text_layer_set_text_color(time_display_layer, GColorWhite);
  
  text_layer_set_font(time_display_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(time_display_layer, GTextAlignmentCenter);
  
  //Add time layer to window's root layer as child layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_display_layer));
  // Register with TickTimerService
tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}




//Function that returns nothing and occurs on window unload
void window_unload(Window *window){
  //Remove image
  gbitmap_destroy(bitmap_image);
  //Remove layer
  bitmap_layer_destroy(background_bitmap_layer);
  //Remove time layer
  text_layer_destroy(time_display_layer);
}

int main(){
  //Create the window on screen
  window = window_create();
  
  //Window handler
  window_set_window_handlers(window,(WindowHandlers){
     //Occurs on load                       
    .load = window_load,
    //Occurs on unload
    .unload = window_unload
  });
  
  window_stack_push(window, true);
  
  app_event_loop();
  
}
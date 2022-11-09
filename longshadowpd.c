/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * 2022 LongShadow Pixel Data (Beaver)
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


enum_start (gegl_long_shadow_style2)
  enum_value      (GEGL_LONG_SHADOW_STYLE_FINITE2,              "finite2",              N_("Finite"))
  enum_value      (GEGL_LONG_SHADOW_STYLE_INFINITE2,            "infinite2",            N_(""))
  enum_value      (GEGL_LONG_SHADOW_STYLE_FADING2,              "fading2",              N_(""))
  enum_value      (GEGL_LONG_SHADOW_STYLE_FADING_FIXED_LENGTH2, "fading-fixed-length2", N_("Fading (fixed length)"))
#ifdef WITH_FADING_FIXED_RATE
  enum_value      (GEGL_LONG_SHADOW_STYLE_FADING_FIXED_RATE2,   "fading-fixed-length-stretch2",   N_("Fading (fixed rate)2"))
#else
  enum_value_skip (GEGL_LONG_SHADOW_STYLE_FADING_FIXED_RATE2)
#endif
enum_end (GeglLongShadowStyle2)

enum_start (gegl_long_shadow_composition2)
  enum_value (GEGL_LONG_SHADOW_COMPOSITION_SHADOW_PLUS_IMAGE2,  "shadow-plus-image2",  N_("Shadow plus image mode"))
  enum_value (GEGL_LONG_SHADOW_COMPOSITION_SHADOW_ONLY2,        "shadow-only2",        N_("Shadow only mode"))
  enum_value (GEGL_LONG_SHADOW_COMPOSITION_SHADOW_MINUS_IMAGE2, "shadow-minus-image2", N_("Shadow minus image mode"))
enum_end (GeglLongShadowComposition2)

property_enum (style, _("Style"),
               GeglLongShadowStyle2, gegl_long_shadow_style2,
               GEGL_LONG_SHADOW_STYLE_FINITE2)
  description (_("Shadow style"))
    ui_meta     ("role", "output-extent")


enum_start (gegl_long_shadow_style3)
  enum_value      (GEGL_LONG_SHADOW_STYLE_FINITE3,              "finite3",              N_("Finite"))
  enum_value      (GEGL_LONG_SHADOW_STYLE_INFINITE3,            "infinite3",            N_(""))
  enum_value      (GEGL_LONG_SHADOW_STYLE_FADING3,              "fading3",              N_(""))
  enum_value      (GEGL_LONG_SHADOW_STYLE_FADING_FIXED_LENGTH3, "fading-fixed-length3", N_("Fading (fixed length)"))
#ifdef WITH_FADING_FIXED_RATE
  enum_value      (GEGL_LONG_SHADOW_STYLE_FADING_FIXED_RATE3,   "fading-fixed-length-stretch3",   N_("Fading (fixed rate)3"))
#else
  enum_value_skip (GEGL_LONG_SHADOW_STYLE_FADING_FIXED_RATE3)
#endif
enum_end (GeglLongShadowStyle3)

enum_start (gegl_long_shadow_composition3)
  enum_value (GEGL_LONG_SHADOW_COMPOSITION_SHADOW_PLUS_IMAGE3,  "shadow-plus-image3",  N_("Shadow plus image mode"))
  enum_value (GEGL_LONG_SHADOW_COMPOSITION_SHADOW_ONLY3,        "shadow-only3",        N_("Shadow only mode"))
  enum_value (GEGL_LONG_SHADOW_COMPOSITION_SHADOW_MINUS_IMAGE3, "shadow-minus-image3", N_("Shadow minus image mode"))
enum_end (GeglLongShadowComposition3)

property_enum (style2, _("Style 2"),
               GeglLongShadowStyle3, gegl_long_shadow_style3,
               GEGL_LONG_SHADOW_STYLE_FADING_FIXED_LENGTH3)
  description (_("Shadow style"))
    ui_meta     ("role", "output-extent")

property_double (angle, _("Angle"), 45.0)
  description (_("Shadow angle"))
  value_range (-180.0, 180.0)
  ui_meta     ("unit", "degree")
  ui_meta     ("direction", "cw")

property_double (length, _("Length"), 40)
  description (_("Shadow length"))
  value_range (0, 55)
  ui_range    (0, 55)


property_enum (composition, _("Composition"),
               GeglLongShadowComposition2, gegl_long_shadow_composition2,
               GEGL_LONG_SHADOW_COMPOSITION_SHADOW_ONLY2)
  description (_("Output composition"))
    ui_meta     ("role", "output-extent")


property_double (lengthblur, _("Length of Pixel Data colors"), 100.0)
    description (_("Length of blur in pixels. Recommended to keep this over double the length of the Long Shadow"))
    value_range (95, 200.0)
    ui_range    (95, 200.0)
    ui_gamma    (1.5)
    ui_meta     ("unit", "pixel-distance")

property_boolean (ls2, _("Fading Long Shadow mode"), FALSE)
  description    (_("Enable a Fading Long Shadow"))

property_double (chroma, _("Chroma"), 0.0)
   description  (_("Chroma adjustment"))
   value_range  (0.0, 30.0)

property_double (lightness, _("Lightness"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (-30.0, 30.0)


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     longshadowpd
#define GEGL_OP_C_SOURCE longshadowpd.c

#include "gegl-op.h"

typedef struct
{
  GeglNode *input;
  GeglNode *alock;
  GeglNode *ls;
  GeglNode *ls2;
  GeglNode *lmb;
  GeglNode *lightchroma;
  GeglNode *output;
} State; 



static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *ls, *ls2, *alock, *lightchroma, *lmb;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  alock    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

  ls    = gegl_node_new_child (gegl,
                                  "operation", "gegl:long-shadow",
                                  NULL);


  ls2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:long-shadow",
                                  NULL);


  lmb    = gegl_node_new_child (gegl,
                                  "operation", "gegl:motion-blur-linear",
                                  NULL);

  lightchroma    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);




  gegl_operation_meta_redirect (operation, "style", ls, "style");
  gegl_operation_meta_redirect (operation, "style2", ls2, "style");
  gegl_operation_meta_redirect (operation, "angle", ls2, "angle");
  gegl_operation_meta_redirect (operation, "angle", ls, "angle"); 
  gegl_operation_meta_redirect (operation, "length", ls, "length"); 
  gegl_operation_meta_redirect (operation, "composition", ls, "composition"); 
  gegl_operation_meta_redirect (operation, "angle", lmb, "angle"); 
  gegl_operation_meta_redirect (operation, "lengthblur", lmb, "length"); 
  gegl_operation_meta_redirect (operation, "chroma", lightchroma, "chroma"); 
  gegl_operation_meta_redirect (operation, "lightness", lightchroma, "lightness"); 




  gegl_node_link_many (input, ls, /*ls2*/ alock, lightchroma, output, NULL);
  gegl_node_link_many (input, lmb, NULL);
 gegl_node_connect_from (alock, "aux", lmb, "output");




 /* Now save points to the various gegl nodes so we can rewire them in
   * update_graph() later
   */
  State *state = g_malloc0 (sizeof (State));
  state->input = input;
  state->ls = ls;
  state->ls2 = ls2;
  state->alock = alock;
  state->lmb = lmb;
  state->lightchroma = lightchroma;
  state->output = output;
  o->user_data = state;
}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  if (o->ls2)
  {
         gegl_node_link_many (state->input, state->ls2, state->alock, state->lightchroma, state->output, NULL);
      gegl_node_link_many (state->input, state->lmb, NULL);
      gegl_node_connect_from (state->alock, "aux", state->lmb, "output");
  }
  else
  {
         gegl_node_link_many (state->input, state->ls, state->alock, state->lightchroma, state->output, NULL);
      gegl_node_link_many (state->input, state->lmb, NULL);
      gegl_node_connect_from (state->alock, "aux", state->lmb, "output");
  }
}







static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class = GEGL_OPERATION_CLASS (klass);
   GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:long-shadow-pd",
    "title",       _("Extrusion via Long Shadow"),
    "categories",  "Artistic",
    "reference-hash", "45ed1636v1f3agbr2549lfl5s001b2ac",
    "description", _("Fork of the existing Long Shadow filter but it uses Pixel Data. You are required to use the behind blend mode. Or run this filter as a duplicate layer below your image.  "
                     ""),
    NULL);
}

#endif

/** Norezator
  *
  * (c) 2011 - 2013 Kyrylo V. Polezhaiev
  */
#include <libgimp/gimp.h>

void               norezator_query                        (void);

void               norezator_run                          (const gchar        *name,
                                                           gint                parameters_count,
                                                           const GimpParam    *parameters,
                                                           gint               *return_values_count,
                                                           GimpParam         **return_values);

GimpPlugInInfo     PLUG_IN_INFO                         = {NULL,
                                                           NULL,
                                                           norezator_query,
                                                           norezator_run};

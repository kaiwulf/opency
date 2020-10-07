
#ifndef __my_marshal_MARSHAL_H__
#define __my_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* BOOLEAN:BOOLEAN (my_marshall.list:1) */
extern void my_marshal_BOOLEAN__BOOLEAN (GClosure     *closure,
                                         GValue       *return_value,
                                         guint         n_param_values,
                                         const GValue *param_values,
                                         gpointer      invocation_hint,
                                         gpointer      marshal_data);

G_END_DECLS

#endif /* __my_marshal_MARSHAL_H__ */


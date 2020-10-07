#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "GObjectDemo"

#include <glib-object.h>

/**************** Structure definitions *************/

/* instance structure */
typedef struct _Media
{
  GObject parent_instance;

  guint inv_nr;
  GString *location;
  GString *title;
  gboolean orig_package;

} Media;

/* class structure */
typedef struct _MediaClass
{
  GObjectClass parent_class;

  /* Signals */
  void (*unpacked)(Media *media);
  void (*throw_out)(Media *media, gboolean permanent);
} MediaClass;

/****************** Utility Macros ******************/

#define TYPE_MEDIA (media_get_type())

#define MEDIA(object) \
  (G_TYPE_CHECK_INSTANCE_CAST((object), TYPE_MEDIA, Media))

#define MEDIA_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_MEDIA, MediaClass))

#define IS_MEDIA(object) \
  (G_TYPE_CHECK_INSTANCE_TYPE((object), TYPE_MEDIA))

#define IS_MEDIA_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_MEDIA))

#define MEDIA_GET_CLASS(object) \
  (G_TYPE_INSTANCE_GET_CLASS((object), TYPE_MEDIA, MediaClass))

/******** Register Media class type identifier *********/

/* prototype for class initializer */
static void media_class_init(MediaClass *class);

/* get the identifier */
GType media_get_type(void)
{
  static GType media_type = 0;

  if (!media_type)
  {
     static const GTypeInfo media_info = {
        sizeof(MediaClass),                /* class structure size */
        NULL,                              /* base class initializer */
        NULL,                              /* base class finalizer */
        (GClassInitFunc)media_class_init,  /* class initializer */
        NULL,                              /* class finalizer */
        NULL,                              /* class data */
        sizeof(Media),                     /* instance structure size */
        16,                                /* preallocated instances */
        NULL,                              /* instance initializer */
        NULL                               /* function table */
     };

    media_type = g_type_register_static(
                    G_TYPE_OBJECT,        /* parent class */
                    "Media",              /* type name */
                    &media_info,          /* GTypeInfo struct (above) */
                    0);                   /* flags */
  }

  return media_type;
}


/********** Media class initialization ***********/

/* Prototypes and constants for property manipulation */

static void media_set_property(GObject *object,
                               guint prop_id,
                               const GValue *value,
                               GParamSpec *pspec);

static void media_get_property(GObject *object,
                               guint prop_id,
                               GValue *value,
                               GParamSpec *pspec);

enum {
  PROP_MEDIA_0,
  PROP_INV_NR,
  PROP_ORIG_PACKAGE
};

/* Signal indexes */
enum
{
  UNPACKED,
  THROW_OUT,
  LAST_SIGNAL
};

/* Signal identifier map */
static guint media_signal[LAST_SIGNAL] = {0, 0};

/* Prototypes for signal handlers */
static void media_unpacked(Media *media);
static void media_throw_out(Media *media, gboolean permanent);

/* Initialize the Media class */
static void media_class_init(MediaClass *class)
{
  GParamSpec *inv_nr_param;
  GParamSpec *orig_package_param;
  GObjectClass *g_object_class;

  /* get handle to base object */
  g_object_class = G_OBJECT_CLASS(class);

  /* << miscellaneous initialization >> */

  /* create GParamSpec descriptions for properties */
  inv_nr_param = g_param_spec_uint("inventory-id",     /* identifier */
                                   "inventory number", /* nickname */
                                   "number on inventory label",
                                                       /* description */
                                   0,                  /* minimum */
                                   UINT_MAX,           /* maximum */
                                   0,                  /* default */
                                   G_PARAM_READWRITE); /* flags */

  orig_package_param = g_param_spec_boolean("orig-package",
                                            "original package?",
                                            "is item in its original package?",
                                            FALSE,
                                            G_PARAM_READWRITE);

  /* override base object methods */
  g_object_class->set_property = media_set_property;
  g_object_class->get_property = media_get_property;

  /* install properties */
  g_object_class_install_property(g_object_class,
                                  PROP_INV_NR,
                                  inv_nr_param);

  g_object_class_install_property(g_object_class,
                                  PROP_ORIG_PACKAGE,
                                  orig_package_param);

  /* set signal handlers */
  class->unpacked = media_unpacked;
  class->throw_out = media_throw_out;

  /* install signals and default handlers */
  media_signal[UNPACKED] =
     g_signal_new("unpacked",                     /* name */
                  TYPE_MEDIA,                     /* class type identifier */
                  G_SIGNAL_RUN_LAST|G_SIGNAL_DETAILED, /* options */
                  G_STRUCT_OFFSET(MediaClass, unpacked), /* handler offset */
                  NULL,                           /* accumulator function */
                  NULL,                           /* accumulator data */
                  g_cclosure_marshal_VOID__VOID,  /* marshaller */
                  G_TYPE_NONE,                    /* type of return value */
                  0);			   

  media_signal[THROW_OUT] =
     g_signal_new("throw-out",
                  TYPE_MEDIA,
                  G_SIGNAL_RUN_LAST|G_SIGNAL_DETAILED,
                  G_STRUCT_OFFSET(MediaClass, throw_out),
                  NULL, NULL,
                  g_cclosure_marshal_VOID__BOOLEAN,
                  G_TYPE_NONE,
                  1,
                  G_TYPE_BOOLEAN);
}

/* set a Media property */
static void media_set_property(GObject *object,
                               guint prop_id,
                               const GValue *value,
                               GParamSpec *pspec)
{
  Media *media;
  guint new_inv_nr;
  gboolean new_orig_package;

  media = MEDIA(object);

  switch(prop_id)
  {
     case PROP_INV_NR:
        new_inv_nr = g_value_get_uint(value);
        if (media->inv_nr != new_inv_nr)
        {
           media->inv_nr = new_inv_nr;
        }
        break;

     case PROP_ORIG_PACKAGE:
        new_orig_package = g_value_get_boolean(value);
        if (media->orig_package != new_orig_package)
        {
           media->orig_package = new_orig_package;
        }
        break;

     default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
  }
}

/* retrive a Media property */
static void media_get_property(GObject *object,
                               guint prop_id,
                               GValue *value,
                               GParamSpec *pspec)
{
  Media *media;

  media = MEDIA(object);
  
  switch(prop_id)
  {
     case PROP_INV_NR:
        g_value_set_uint(value, media->inv_nr);
        break;

     case PROP_ORIG_PACKAGE:
        g_value_set_boolean(value, media->orig_package);
        break;
 
     default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
  }
}

/* a sample method */
void media_print_inv_nr(Media *object)
{
  Media *media;

  g_return_if_fail(IS_MEDIA(object));

  media = MEDIA(object);
  g_print("Inventory number: %d\n", media->inv_nr);
}

/* unpacked signal handler */
static void media_unpacked(Media *media)
{
  if (media->orig_package)
  {
     g_object_set(media, "orig-package", FALSE, NULL);
     g_print("Media unpacked.\n");
  } else {
     g_print("Media already unpacked.\n");
  }
}

/* throw-out signal handler */
static void media_throw_out(Media *media, gboolean permanent)
{
  if (permanent)
  {
     g_print("Trashing media.\n");
  } else {
     g_print("Media not in the dumpster quite yet.\n");
  }
}

/* These signal handlers test handlers connected after installation;
   see main program */
static void meep_meep(Media *media);
static void ding(Media *media);
static void dong(Media *media);

static void meep_meep(Media *media)
{
  guint nr;
  g_object_get(media, "inventory-id", &nr, NULL);

  g_print("Meep-meep! (Inventory number: \%d)\n", nr);
}

static void ding(Media *media)
{
  g_print("Ding.\n");
}

static void dong(Media *media)
{
  g_print("Dong.\n");
}

/* A hook */

static gboolean my_hook(GSignalInvocationHint *ihint,
                        guint n_param_values,
			const GValue *param_values,
			gpointer *data);

static gboolean my_hook(GSignalInvocationHint *ihint,
                        guint n_param_values,
			const GValue *param_values,
			gpointer *data)
{
  static gint n = 0;
  guint inv_nr;
  Media *m;
  GObject *obj;

  g_print("my_hook(): ");

  /* check for a valid Media object */
  if (n_param_values > 0)
  {
     obj = g_value_get_object(param_values + 0);
     if (IS_MEDIA(obj))
     {
	m = MEDIA(obj);
        g_object_get(m, "inventory-id", &inv_nr, NULL);
        g_print("inventory number = %d.\n", inv_nr);
     } else {
        g_print("called with invalid object\n");
     }
  } else {
      g_print("called with invalid parameters\n");
  }

  n++;
  g_print("my_hook(): invocation #%d", n);

  if (n == 3)
  {
     g_print(" (last time)\n");
     return(FALSE);
  } else {
     g_print("\n");
     return(TRUE);
  }
}

/***** Define Cleanable macros, structures, and prototypes *****/
/*          (actual interface later down in the file)          */

GType cleanable_get_type() G_GNUC_CONST;

#define TYPE_CLEANABLE (cleanable_get_type())

#define CLEANABLE(object) \
  (G_TYPE_CHECK_INSTANCE_CAST((object),  TYPE_CLEANABLE, Cleanable)) 

#define IS_CLEANABLE(object) \
  (G_TYPE_CHECK_INSTANCE_TYPE((object), TYPE_CLEANABLE))

#define CLEANABLE_GET_CLASS(object) \
  (G_TYPE_INSTANCE_GET_INTERFACE((object), TYPE_CLEANABLE, CleanableClass))

/* empty declaration for instance structure */
typedef struct _Cleanable Cleanable;

/* Cleanable class structure */
typedef struct _CleanableClass
{
  GTypeInterface base_interface;
  void (*clean) (Cleanable *object);
} CleanableClass;


/******** CD (class derived from Media) **********/

typedef struct _CD {
  Media media_instance;
  gboolean writable;
} CD;

typedef struct _CDClass {
  MediaClass media_class;
} CDClass;

#define TYPE_CD (cd_get_type())
#define CD(object) \
   (G_TYPE_CHECK_INSTANCE_CAST((object), TYPE_CD, CD))

#define CD_CLASS(klass) \
   (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_CD, CDClass))

#define IS_CD(object) \
   (G_TYPE_CHECK_INSTANCE_TYPE((object), TYPE_CD))

#define IS_CD_CLASS(klass) \
   (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_CD))

#define CD_GET_CLASS(object) \
   (G_TYPE_INSTANCE_GET_CLASS((object), TYPE_CD, CDClass))

static void cd_class_init(CDClass *class);

static void cd_cleanable_init(gpointer interface, gpointer data);

/* CD Type initializer */
GType cd_get_type(void)
{
  static GType cd_type = 0;
  const GInterfaceInfo cleanable_info;

  if (!cd_type)
  {
     static const GTypeInfo cd_info = {
        sizeof(CDClass),
        NULL,
        NULL,
        (GClassInitFunc)cd_class_init,
        NULL,
        NULL,
        sizeof(CD),
        16,
        NULL
     };

     const GInterfaceInfo cleanable_info = {
        cd_cleanable_init, NULL, NULL
     };

     /* Register type, use ID of parent class TYPE_MEDIA */
     /* "CD" is too short, use "CompactDisc" instead */
     cd_type = g_type_register_static(TYPE_MEDIA, "CompactDisc", &cd_info, 0);

     /* add interface */
     g_type_add_interface_static(cd_type, TYPE_CLEANABLE, &cleanable_info);
  }
  return cd_type;
}

/* CD constants and prototypes for properties */
enum {
  PROP_0_CD,
  PROP_WRITABLE
};

static void cd_get_property(GObject *object,
                            guint prop_id,
                            GValue *value,
                            GParamSpec *pspec);

static void cd_set_property(GObject *object,
                            guint prop_id,
                            const GValue *value,
                            GParamSpec *pspec);

/* a new default signal handler for unpacked */
static void unpacked_cd()
{
  g_print("Hi!\n");
}

/* CD class initializer */
static void cd_class_init(CDClass *class)
{
  GObjectClass *g_object_class;
  MediaClass *media_class;
 
  media_class = MEDIA_CLASS(class);
  media_class->unpacked = unpacked_cd;
  
  g_object_class = G_OBJECT_CLASS(class);
  g_object_class->set_property = cd_set_property;
  g_object_class->get_property = cd_get_property;

  g_object_class_install_property(
     g_object_class,
     PROP_WRITABLE,
     g_param_spec_boolean("writable", "Writable?",
                          "Is the CD writable?", FALSE,
                          G_PARAM_READWRITE|G_PARAM_CONSTRUCT_ONLY));
}

static void cd_set_property(GObject *object,
                            guint prop_id,
                            const GValue *value,
                            GParamSpec *pspec)
{
  gboolean writable;
  CD *cd = CD(object);

  switch(prop_id)
  {
     case PROP_WRITABLE:
        writable = g_value_get_boolean(value);
        if (cd->writable != writable)
        {
           cd->writable = writable;
        }
        break;

     default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
  }
}

static void cd_get_property(GObject *object,
                            guint prop_id,
                            GValue *value,
                            GParamSpec *pspec)
{
  CD *cd = CD(object);

  switch(prop_id)
  {
     case PROP_WRITABLE:
        g_value_set_boolean(value, cd->writable);
        break;

     default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
  }
}


/**************** EightTrack subclass ****************/

typedef struct _EightTrack {
  Media media_instance;
  gint minutes;
} EightTrack;

typedef struct _EightTrackClass {
  MediaClass media_class;
} EightTrackClass;

#define TYPE_EIGHTTRACK (eighttrack_get_type())
#define EIGHTTRACK(object) \
  (G_TYPE_CHECK_INSTANCE_CAST((object), TYPE_EIGHTTRACK, EightTrack))

#define EIGHTTRACK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_EIGHTTRACK, EightTrackClass))

#define IS_EIGHTTRACK(object) \
  (G_TYPE_CHECK_INSTANCE_TYPE((object), TYPE_EIGHTTRACK))

#define IS_EIGHTTRACK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_EIGHTTRACK))

#define EIGHTTRACK_GET_CLASS(object) \
  (G_TYPE_INSTANCE_GET_CLASS((object), TYPE_EIGHTTRACK, EightTrackClass))

static void eighttrack_class_init(EightTrackClass *class);

static void eighttrack_cleanable_init(gpointer interface, gpointer data);

GType eighttrack_get_type(void)
{
  static GType eighttrack_type = 0;

  if (!eighttrack_type)
  {
     const GTypeInfo eighttrack_info = {
        sizeof(EightTrackClass),
        NULL, NULL,
        (GClassInitFunc)eighttrack_class_init,
        NULL, NULL,
        sizeof(EightTrack), 16,
        NULL
     };

     const GInterfaceInfo cleanable_info =
        { eighttrack_cleanable_init, NULL, NULL };

     /* Register class type, ID of parent is TYPE_MEDIA */
     eighttrack_type = g_type_register_static(TYPE_MEDIA,
                                              "EightTrack",
                                              &eighttrack_info,
                                              0);

     g_type_add_interface_static(eighttrack_type,
				 TYPE_CLEANABLE,
                                 &cleanable_info);
  }
  return (eighttrack_type);
}

enum {
  PROP_0_EIGHTTRACK,
  PROP_MINUTES
};

static void eighttrack_set_property(GObject *object,
                                    guint prop_id,
                                    const GValue *value,
                                    GParamSpec *pspec)
{
  EightTrack *eighttrack;
  guint n;

  eighttrack = EIGHTTRACK(object);

  switch(prop_id)
  {
     case PROP_MINUTES:
        n = g_value_get_uint(value);
        if (eighttrack->minutes != n)
        {
           eighttrack->minutes = n;
           g_object_notify(G_OBJECT(eighttrack), "minutes");
        }
        break;

     default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
  }
}

static void eighttrack_get_property(GObject *object,
                                  guint prop_id,
                                  GValue *value,
                                  GParamSpec *pspec)
{
  EightTrack *eighttrack;
  eighttrack = EIGHTTRACK(object);

  switch(prop_id)
  {
     case PROP_MINUTES:
        g_value_set_uint(value, eighttrack->minutes);
        break;

     default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
  }
}

static void eighttrack_class_init(EightTrackClass *class)
{
  GObjectClass *g_object_class;
 
  g_object_class = G_OBJECT_CLASS(class);
  g_object_class->set_property =
	eighttrack_set_property;
  g_object_class->get_property =
	eighttrack_get_property;

  g_object_class_install_property(
      g_object_class,
      PROP_MINUTES,
      g_param_spec_uint("minutes",
                        "playing-time",
                        "Playing Time of 8-Track Tape in Minutes",
                         20, 80, 60,  /* min, max, default */
                         G_PARAM_READWRITE|G_PARAM_CONSTRUCT_ONLY));
}

/******** Finish "Cleanable" interface **********/

static guint cleanable_base_init_count = 0;

static void cleanable_base_init(CleanableClass *cleanable)
{
  cleanable_base_init_count++;

  if (cleanable_base_init_count == 1)
  {
     /* "constructor" code, for example, register signals */
  }
}

static void cleanable_base_finalize(CleanableClass *cleanable) {
  cleanable_base_init_count--;

  if (cleanable_base_init_count == 0)
  {
     /* "destructor" code, for example, unregister signals */
  }
}

GType cleanable_get_type(void)
{
  static GType cleanable_type = 0;

  if (!cleanable_type)
  {
     static const GTypeInfo cleanable_info = {
        sizeof(CleanableClass),
        (GBaseInitFunc) cleanable_base_init,
        (GBaseFinalizeFunc) cleanable_base_finalize
     };

     cleanable_type = g_type_register_static(G_TYPE_INTERFACE,
                                             "Cleanable",
                                             &cleanable_info,
                                             0);
  }

  return cleanable_type;
}

void cd_clean(Cleanable *cleanable)
{
  IS_CD(CD(cleanable));
  
  g_print("Cleaning CD.\n");
}

void eighttrack_clean(Cleanable *cleanable)
{
  g_assert(IS_EIGHTTRACK(EIGHTTRACK(cleanable)));
  
  g_print("Cleaning 8-Track tape.\n");
}


static void cd_cleanable_init(gpointer interface, gpointer data)
{
  CleanableClass *cleanable = interface;

  g_assert(G_TYPE_FROM_INTERFACE(cleanable) == TYPE_CLEANABLE);
  /* is the interface ready? */
  g_assert(cleanable_base_init_count > 0);
 
  cleanable->clean = cd_clean;
}

static void eighttrack_cleanable_init(gpointer interface, gpointer data)
{
  CleanableClass *cleanable = interface;

  g_assert(G_TYPE_FROM_INTERFACE(cleanable) == TYPE_CLEANABLE);
  g_assert(cleanable_base_init_count > 0);
 
  cleanable->clean = eighttrack_clean;
}

void clean(Cleanable *object)
{
  CleanableClass *interface;

  g_return_if_fail(IS_CLEANABLE(object));

  interface = CLEANABLE_GET_CLASS(object);
  g_object_ref(object);
  interface->clean(object);
  g_object_unref(object);
}

int main(int argc, char **argv)
{
  Media *media, *media2, *media_weak, *m2;
  CD *cd;
  EightTrack *eighttrack;
  guint min, nr, i, *sigs;
  gboolean is_unpacked;
  Cleanable *cleanable;
  gulong handler_id;
  gulong hook_id;

  g_type_init();

  /* --- create objects --- */

  g_print("\n* creating objects...\n");

  /* create an object, setting some properties */
  media = g_object_new(TYPE_MEDIA,
                       "inventory-id", 42,
                       "orig-package", FALSE,
                       NULL);

  /* --- set and retrieve properties --- */

  g_print("\n* Testing properties...\n");

  /* get previous values */
  g_print("Previous values: ");
  g_object_get(media,
              "orig-package", &is_unpacked,
              "inventory-id", &nr,
              NULL);
  g_print("inventory-id = %d, orig-package = %s\n",
	   nr, is_unpacked ? "TRUE" : "FALSE");

  /* set new values */
  g_print("Setting inventory-id = 37, orig-package = TRUE\n");
  g_object_set(media,
	       "orig-package", TRUE,
               "inventory-id", 37,
               NULL);

  /* double-check those new values */
  g_print("Verifying.. ");
  g_object_get(media,
              "orig-package", &is_unpacked,
               "inventory-id", &nr,
               NULL);
  g_print("inventory-id = %d, orig-package = %s\n",
	   nr, is_unpacked ? "TRUE" : "FALSE");

  /* --- methods --- */

  g_print("\n* Testing methods...\n");

  /* call example method; expect "Inventory number: 37" */
  media_print_inv_nr(media);

  /* --- signals --- */

  g_print("\n* Testing signals...\n");

  g_signal_emit_by_name(media, "unpacked");
  /* expect "Media unpacked." */

  g_signal_emit_by_name(media, "unpacked");
  /* expect "Media already unpacked." */

  g_signal_emit(media, media_signal[UNPACKED], 0);
  /* expect "Media already unpacked." */

  g_signal_emit_by_name(media, "throw-out", TRUE);
  /* expect "Trashing media." */

  /* Note that in this example program, "throw-out" only prints
     a message. It doesn't actually mark the object for deletion
     or anything. */

  /* SIGNAL RETURN VALUES:
   * If you'd like throw-out to return gboolean:
   *
   * 1. change the function and its prototype
   * 2. change the signal definition in the media class structure
   * 3. add a marshaller with glib-genmarshal
   * 4. change signal installer in the class initializer (media_class_init):
   *    a. replace the marshaller with the new one from step 3
   *    b. change the return type from G_TYPE_NONE to G_TYPE_BOOLEAN
   *
   * Then, use something like this to get a return value:
   *
   * gboolean return_val;
   * ...
   *
   * g_signal_emit_by_name(media, "throw-out", TRUE, &return_val);
   *
   * if (return_val)
   * {
   *    g_print("Signal (throw-out): returned TRUE.\n");
   * } else {
   *    g_print("Signal (throw-out): returned FALSE.\n");
   * }
   *
   */

  /* user-defined signal handlers */

  g_print(" - User-defined handlers...\n");
  /* connect new handler */
  handler_id = g_signal_connect(media, "unpacked", (GCallback) meep_meep, NULL);

  g_signal_emit_by_name(media, "unpacked");
  /* expect "Meep Meep (...)" then "Media already unpacked */

  /* test and disconnect handlers */
  if (g_signal_handler_is_connected(media, handler_id))
  {
     g_print("meepmeep is connected to media. Detaching...\n");
  }

  g_signal_handler_disconnect(media, handler_id);

  if (!g_signal_handler_is_connected(media, handler_id))
  {
     g_print("meepmeep no longer connected:\n");
     g_signal_emit_by_name(media, "unpacked");
  }

  /* signal details */
  g_print(" - details...\n");

  /* connect handlers with ding and dong details */
  g_signal_connect(media, "unpacked::ding", (GCallback)ding, NULL);

  g_signal_connect(media, "unpacked::dong", (GCallback)dong, NULL);

  g_signal_emit_by_name(media, "unpacked::ding");
  /* expect "Ding," then "Media ... unpacked" */

  g_signal_emit_by_name(media, "unpacked::dong");
  /* expect "Dong," then "Media ... unpacked" */

  g_signal_emit_by_name(media, "unpacked");
  /* expect only "Media ... unpacked" */


  /* signal hooks */
  g_print(" - hooks...\n");

  /* create two new objects */
  m2 = g_object_new(TYPE_MEDIA,
                    "inventory-id", 98,
                    "orig-package", TRUE,
                    NULL);

  /* add an emissions hook */
  hook_id = g_signal_add_emission_hook(media_signal[UNPACKED],
                                       0,
                                       (GSignalEmissionHook)my_hook,
                                       NULL, NULL);

  /* test the hook on three different objects */
  g_signal_emit_by_name(media, "unpacked");
  g_signal_emit_by_name(m2, "unpacked");
  g_signal_emit_by_name(media, "unpacked");

  /* this time, the hook should no longer be active */
  g_signal_emit_by_name(media, "unpacked");

  g_print(" - check all signals...\n");

  sigs = g_signal_list_ids(TYPE_MEDIA, &nr);
  g_print("ID    Name\n");
  g_print("----  -------------\n");
  i = 0;
  while (i < nr)
  {
     g_print("%-4d  %s\n", *sigs, g_signal_name(*sigs));
     i++;
     sigs++;
  }
  g_print("\nTotal signals: %d\n", nr);
  g_free(sigs);


  /* --- subclasses --- */

  g_print("\n* Testing subclasses...\n");

  /* create a new CD object */
  cd = g_object_new(TYPE_CD, "inventory-id", 423, "writable", FALSE, NULL);

  /* verify data in the object */
  g_object_get(cd, "inventory-id", &nr, "writable", &is_unpacked, NULL);

  g_print("cd: writable = %s, inventory-id = %d\n",
           is_unpacked? "true":"false", nr);

  /* create an EightTrack object */
  eighttrack = g_object_new(TYPE_EIGHTTRACK, "minutes", 47, NULL);

  /* verify playing time */
  g_object_get(eighttrack, "minutes", &min, NULL);
  g_print("eighttrack: playing time = %d min\n", min);

  g_print(" - Testing signal handlers...\n");

  /* EightTrack's unpacked handler; same as Media's */
  g_signal_emit_by_name(eighttrack, "unpacked", NULL);

  /* CD's unpack handler; expect "Hi!" instead */
  g_signal_emit_by_name(cd, "unpacked", NULL);


  /* --- Test Membership --- */

  /* see what objects belong to what classes */
  g_print(" - Testing membership...\n");

  /* is cd in Media? (subclass in parent; expect true) */
  g_print("cd is %sMedia object\n", IS_MEDIA(cd)? "a " : "not a ");

  /* is eighttrack in Media? (subclass in parent; expect true) */
  g_print("eighttrack is %sMedia object \n", IS_MEDIA(eighttrack)? "a " : "not a ");

  /* is media in CD? (parent in subclass; expect false) */
  g_print("media is %sCD object\n", IS_CD(media)? "a " : "not a ");

  /* is cd in EightTrack? (expect false) */
  g_print("cd is %sEightTrack object\n", IS_EIGHTTRACK(cd)? "an " : "not an ");

  g_print(" - Testing Cleanable interface...\n");

  clean(CLEANABLE(cd));
  clean(CLEANABLE(eighttrack));

  /* --- references --- */

  g_print("\n* Testing references...\n");
  /* set media2 to a strong reference from media */
  media2 = g_object_ref(media);

  /* set media_weak to a weak reference to media2 */
  media_weak = media2;
  g_object_add_weak_pointer(G_OBJECT(media2), (gpointer) &media_weak);

  /* remove one strong reference */
  g_object_unref(media2);

  /* see if media_weak is NULL, meaning that object is gone */
  if (media_weak == NULL)
  {
     g_print("media_weak is NULL; object is gone\n");
  } else {
     g_print("media_weak is not NULL; object is still in memory\n");
  }

  /* remove another strong reference */
  g_object_unref(media);

  /* check the weak pointer again */
  if (media_weak == NULL)
  {
     g_print("media_weak is NULL; object is gone\n");
  }

  g_object_unref(m2);

  return 0;
}

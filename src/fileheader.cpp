#include <sstream>
#include <zim/fileheader.h>
#include "fileheader.h"
#include "file.h"

/**
 * SECTION: zim-fileheader
 * @Title: ZimFileheader
 * @short_description: A zim fileheader
 *
 * #ZimFileheader contains some informations of the zim file.
 */

#define ZIM_FILEHEADER_GET_PRIVATE(obj) \
    ((ZimFileheaderPrivate *) zim_fileheader_get_instance_private ((ZimFileheader *) (obj)))

typedef struct _ZimFileheaderPrivate ZimFileheaderPrivate;
struct _ZimFileheaderPrivate
{
    ZimFile *zim_file;
    zim::Fileheader fileheader;
};

G_DEFINE_TYPE_WITH_PRIVATE (ZimFileheader, zim_fileheader, G_TYPE_OBJECT)


static void
zim_fileheader_finalize (GObject *gobject)
{
    ZimFileheaderPrivate *priv = ZIM_FILEHEADER_GET_PRIVATE (gobject);

    g_object_unref (priv->zim_file);

    G_OBJECT_CLASS (zim_fileheader_parent_class)->dispose (gobject);
}

static void
zim_fileheader_class_init (ZimFileheaderClass *klass)
{
    G_OBJECT_CLASS (klass)->finalize = zim_fileheader_finalize;
}

static void
zim_fileheader_init (ZimFileheader *object)
{
}

void
zim_fileheader_set_internal_fileheader (ZimFileheader *fileheader, ZimFile *zim_file, const zim::Fileheader fileheader_cpp)
{
    ZimFileheaderPrivate *priv = ZIM_FILEHEADER_GET_PRIVATE (fileheader);

    priv->zim_file = zim_file;
    g_object_ref (zim_file);

    priv->fileheader = fileheader_cpp;
}

/**
 * zim_fileheader_new:
 *
 * Allocates a new #ZimFileheader.
 *
 * Returns: (transfer full): the newly created #ZimFileheader instance
 */
ZimFileheader *
zim_fileheader_new (void)
{
    ZimFileheader *fileheader;
    fileheader = (ZimFileheader*) g_object_new (ZIM_TYPE_FILEHEADER, NULL);

    ZimFileheaderPrivate *priv = ZIM_FILEHEADER_GET_PRIVATE (fileheader);
    priv->fileheader = zim::Fileheader();

    return fileheader;
}

/**
 * zim_fileheader_has_main_page:
 * @fileheader: A #ZimFileheader
 *
 * Check if the zim file has a main page.
 *
 * Returns: TRUE if the zim file hash main page, FALSE otherwise
 */
gboolean
zim_fileheader_has_main_page (ZimFileheader* fileheader)
{
    ZimFileheaderPrivate *priv = ZIM_FILEHEADER_GET_PRIVATE (fileheader);
    return priv->fileheader.hasMainPage ();
}

/**
 * zim_fileheader_get_main_page:
 * @fileheader: A #ZimFileheader
 *
 * Get the index of the main page.
 *
 * Returns: the index of the main page
 */
unsigned int
zim_fileheader_get_main_page (ZimFileheader* fileheader)
{
    ZimFileheaderPrivate *priv = ZIM_FILEHEADER_GET_PRIVATE (fileheader);
    return priv->fileheader.getMainPage ();
}

/**
 * zim_fileheader_get_uuid:
 * @fileheader: A #ZimFileheader
 *
 * Get the uuid of the zim file.
 *
 * Returns: the uuid of the zim file
 */
const char *
zim_fileheader_get_uuid (ZimFileheader* fileheader)
{
    ZimFileheaderPrivate *priv = ZIM_FILEHEADER_GET_PRIVATE (fileheader);
    std::ostringstream s;
    s << priv->fileheader.getUuid();
    std::string uuid = s.str();
    return g_strdup (uuid.c_str ());
}

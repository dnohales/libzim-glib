#include <zim/file.h>
#include <zim/fileiterator.h>
#include <zim/article.h>
#include "file.h"
#include "fileheader.h"
#include "fileheader-private.h"
#include "article.h"
#include "article-private.h"
#include "file-iterator.h"
#include "file-iterator-private.h"

/**
 * SECTION: zim-file
 * @Title: ZimFile
 * @short_description: A zim file
 *
 * #ZimFile class is the root class to access to the articles of the zim file.
 */

#define ZIM_FILE_GET_PRIVATE(obj) \
    ((ZimFilePrivate *) zim_file_get_instance_private ((ZimFile *) (obj)))

typedef struct _ZimFilePrivate ZimFilePrivate;
struct _ZimFilePrivate
{
    zim::File *file;
};

G_DEFINE_TYPE_WITH_PRIVATE (ZimFile, zim_file, G_TYPE_OBJECT)

static void
zim_file_finalize (GObject *gobject)
{
    ZimFilePrivate *priv = ZIM_FILE_GET_PRIVATE (gobject);

    delete priv->file;

    G_OBJECT_CLASS (zim_file_parent_class)->dispose (gobject);
}

static void
zim_file_class_init (ZimFileClass *klass)
{
    G_OBJECT_CLASS (klass)->finalize = zim_file_finalize;
}

static void
zim_file_init (ZimFile *object)
{
}

zim::File *
zim_file_get_internal_zim_file (ZimFile *zim_file)
{
    ZimFilePrivate *priv = ZIM_FILE_GET_PRIVATE (zim_file);
    return priv->file;
}

/**
 * zim_file_new:
 * @zimpath: the path to the zim file
 * @error: a #GError object
 *
 * Allocates a new #ZimFile.
 *
 * Returns: (transfer full): the newly created #ZimFile instance
 */
ZimFile *
zim_file_new (const char *zimpath, GError **error)
{
    ZimFile *file;
    file = (ZimFile*) g_object_new (ZIM_TYPE_FILE, NULL);

    ZimFilePrivate *priv = ZIM_FILE_GET_PRIVATE (file);
    try
    {
        priv->file = new zim::File (zimpath);
    }
    catch (const std::exception &err)
    {
        *error = g_error_new_literal (1, 0, err.what ());
    }

    return file;
}

/**
 * zim_file_get_namespace_begin_offset:
 * @file: A #ZimFile
 * @namesp: the namespace
 *
 * Get the offset of the beginning of the namespace.
 *
 * Returns: the offset of the namespace
 */
unsigned int
zim_file_get_namespace_begin_offset (ZimFile *file, char namesp)
{
    ZimFilePrivate *priv = ZIM_FILE_GET_PRIVATE (file);
    unsigned int index = priv->file->getNamespaceBeginOffset (namesp);
    return index;
}

/**
 * zim_file_get_namespace_count:
 * @file: A #ZimFile
 * @namesp: the namespace
 *
 * Get the count of entries in the namespace.
 *
 * Returns: the count of entries
 */
unsigned int
zim_file_get_namespace_count (ZimFile *file, char namesp)
{
    ZimFilePrivate *priv = ZIM_FILE_GET_PRIVATE (file);
    unsigned int count = priv->file->getNamespaceCount (namesp);
    return count;
}

/**
 * zim_file_get_fileheader:
 * @file: A #ZimFile
 *
 * Get the #ZimFileheader object of the zim file.
 *
 * Returns: (transfer full): the #ZimFileheader
 */
ZimFileheader *
zim_file_get_fileheader (ZimFile *file)
{
    ZimFilePrivate *priv = ZIM_FILE_GET_PRIVATE (file);

    ZimFileheader* fileheader = zim_fileheader_new ();
    zim_fileheader_set_internal_fileheader (fileheader, file, priv->file->getFileheader ());

    return fileheader;
}

/**
 * zim_file_get_article_by_index:
 * @file: A #ZimFile
 * @index: index of an article
 *
 * Get the article at the given index.
 *
 * Returns: (transfer full): the #ZimArticle at the given index
 */
ZimArticle *
zim_file_get_article_by_index (ZimFile *file, unsigned int index)
{
    ZimFilePrivate *priv = ZIM_FILE_GET_PRIVATE (file);

    ZimArticle *article = zim_article_new ();
    zim::Article article_cpp = priv->file->getArticle (index);
    zim_article_set_internal_article (article, file, article_cpp);

    return article;
}

/**
 * zim_file_get_article_by_namespace:
 * @file: A #ZimFile
 * @namesp: the namespace
 * @path: the path of the article inside the namespace
 *
 * Get an article knowing its path inside a namespace.
 *
 * Returns: (transfer full): the #ZimArticle found
 */
ZimArticle *
zim_file_get_article_by_namespace (ZimFile *file, const char namesp, const char *path)
{
    ZimFilePrivate *priv = ZIM_FILE_GET_PRIVATE (file);

    ZimArticle *article = zim_article_new ();
    zim::Article article_cpp = priv->file->getArticle (namesp, path);
    zim_article_set_internal_article (article, file, article_cpp);

    return article;
}

/**
 * zim_file_find_by_title:
 * @file: A #ZimFile
 * @namesp: the namespace
 * @title: title
 *
 * Start search of articles corresponding to a title.
 *
 * Returns: (transfer full): the #ZimFileIterator
 */
ZimFileIterator *
zim_file_find_by_title (ZimFile *file, const char namesp, const char *title)
{
    ZimFilePrivate *priv = ZIM_FILE_GET_PRIVATE (file);

    ZimFileIterator *file_iterator = zim_file_iterator_new ();
    zim::File::const_iterator file_iterator_cpp = priv->file->findByTitle (namesp, title);
    zim_file_iterator_set_internal_file_iterator (file_iterator, file, file_iterator_cpp);

    return file_iterator;
}

/**
 * zim_file_get_filesize:
 * @file: A #ZimFile
 *
 * Get file size of the archive.
 *
 * Returns: the size
 */
unsigned long
zim_file_get_filesize (ZimFile *file)
{
    ZimFilePrivate *priv = ZIM_FILE_GET_PRIVATE (file);
    unsigned long size = priv->file->getFilesize ();
    return size;
}

/**
 * zim_file_get_count_articles:
 * @file: A #ZimFile
 *
 * Get the global count of entries.
 *
 * Returns: the global count of entries
 */
unsigned int
zim_file_get_count_articles (ZimFile *file)
{
    ZimFilePrivate *priv = ZIM_FILE_GET_PRIVATE (file);
    unsigned int count = priv->file->getCountArticles ();
    return count;
}

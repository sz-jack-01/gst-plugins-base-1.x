/*
 * GStreamer
 * Copyright (C) 2012 Collabora Ltd.
 *   @author: Sebastian Dröge <sebastian.droege@collabora.co.uk>
 * Copyright (C) 2014 Julien Isorce <julien.isorce@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef _GST_EGL_IMAGE_H_
#define _GST_EGL_IMAGE_H_

#include <gst/gl/gstgl_fwd.h>
#include <gst/gl/gstglformat.h>

#include <libdrm/drm_fourcc.h>

G_BEGIN_DECLS

#ifndef DRM_FORMAT_NV15
#define DRM_FORMAT_NV15 fourcc_code('N', 'V', '1', '5')
#endif

#ifndef DRM_FORMAT_YUV420_8BIT
#define DRM_FORMAT_YUV420_8BIT fourcc_code('Y', 'U', '0', '8')
#endif

#ifndef DRM_FORMAT_YUV420_10BIT
#define DRM_FORMAT_YUV420_10BIT fourcc_code('Y', 'U', '1', '0')
#endif

#ifndef DRM_FORMAT_MOD_VENDOR_ARM
#define DRM_FORMAT_MOD_VENDOR_ARM 0x08
#endif

#ifndef DRM_FORMAT_MOD_ARM_AFBC
#define DRM_FORMAT_MOD_ARM_AFBC(__afbc_mode) fourcc_mod_code(ARM, __afbc_mode)
#endif

#ifndef AFBC_FORMAT_MOD_BLOCK_SIZE_16x16
#define AFBC_FORMAT_MOD_BLOCK_SIZE_16x16 (1ULL)
#endif

#ifndef AFBC_FORMAT_MOD_SPARSE
#define AFBC_FORMAT_MOD_SPARSE (((__u64)1) << 6)
#endif

#define DRM_AFBC_MODIFIER \
  (DRM_FORMAT_MOD_ARM_AFBC(AFBC_FORMAT_MOD_SPARSE) | \
   DRM_FORMAT_MOD_ARM_AFBC(AFBC_FORMAT_MOD_BLOCK_SIZE_16x16))

#ifndef GST_VIDEO_FLAG_ARM_AFBC
#define GST_VIDEO_FLAG_ARM_AFBC (1UL << 31)
#define GST_VIDEO_INFO_SET_AFBC(i) \
  GST_VIDEO_INFO_FLAG_SET (i, GST_VIDEO_FLAG_ARM_AFBC)
#define GST_VIDEO_INFO_UNSET_AFBC(i) \
  GST_VIDEO_INFO_FLAG_UNSET (i, GST_VIDEO_FLAG_ARM_AFBC)
#define GST_VIDEO_INFO_IS_AFBC(i) \
  GST_VIDEO_INFO_FLAG_IS_SET (i, GST_VIDEO_FLAG_ARM_AFBC)
#endif

GST_GL_API GType gst_egl_image_get_type (void);

#define GST_TYPE_EGL_IMAGE                         (gst_egl_image_get_type())
#define GST_IS_EGL_IMAGE(obj)                      (GST_IS_MINI_OBJECT_TYPE(obj, GST_TYPE_EGL_IMAGE))
#define GST_EGL_IMAGE_CAST(obj)                    ((GstEGLImage *)(obj))
#define GST_EGL_IMAGE(obj)                         (GST_EGL_IMAGE_CAST(obj))

typedef struct _GstEGLImage GstEGLImage;

/**
 * GstEGLImageDestroyNotify:
 * @image: a #GstEGLImage
 * @data: user data passed to gst_egl_image_new_wrapped()
 *
 * Function to be called when the GstEGLImage is destroyed. It should free
 * the associated `EGLImage` if necessary
 */
typedef void (*GstEGLImageDestroyNotify) (GstEGLImage * image,
    gpointer data);

/**
 * GstEGLImage:
 *
 * Opaque #GstEGLImage struct.
 */
struct _GstEGLImage
{
  /*< private >*/
  GstMiniObject parent;

  GstGLContext *context;
  gpointer image;
  GstGLFormat format;

  gpointer destroy_data;
  GstEGLImageDestroyNotify destroy_notify;

  gpointer _padding[GST_PADDING];
};

GST_GL_API
GstEGLImage *             gst_egl_image_new_wrapped             (GstGLContext * context,
                                                                 gpointer image,
                                                                 GstGLFormat format,
                                                                 gpointer user_data,
                                                                 GstEGLImageDestroyNotify user_data_destroy);
GST_GL_API
gpointer                gst_egl_image_get_image                 (GstEGLImage * image);

GST_GL_API
GstEGLImage *           gst_egl_image_from_texture              (GstGLContext * context,
                                                                 GstGLMemory * gl_mem,
                                                                 guintptr * attribs);
#if GST_GL_HAVE_DMABUF
GST_GL_API
GstEGLImage *           gst_egl_image_from_dmabuf               (GstGLContext * context,
                                                                 gint dmabuf,
                                                                 const GstVideoInfo * in_info,
                                                                 gint plane,
                                                                 gsize offset);
GST_GL_API
GstEGLImage *           gst_egl_image_from_dmabuf_direct        (GstGLContext * context,
                                                                 gint *fd,
                                                                 const gsize *offset,
                                                                 const GstVideoInfo * in_info);
GST_GL_API
GstEGLImage *           gst_egl_image_from_dmabuf_direct_target (GstGLContext * context,
                                                                 gint *fd,
                                                                 const gsize *offset,
                                                                 const GstVideoInfo * in_info,
                                                                 GstGLTextureTarget target);

GST_GL_API
gboolean                gst_egl_image_export_dmabuf             (GstEGLImage *image, int *fd, gint *stride, gsize *offset);
#endif

/**
 * gst_egl_image_ref:
 * @image: a #GstEGLImage.
 *
 * Increases the refcount of the given image by one.
 *
 * Returns: (transfer full): @image
 */
static inline GstEGLImage *
gst_egl_image_ref (GstEGLImage * image)
{
  return (GstEGLImage *) gst_mini_object_ref (GST_MINI_OBJECT_CAST (image));
}

/**
 * gst_egl_image_unref:
 * @image: (transfer full): a #GstEGLImage.
 *
 * Decreases the refcount of the image. If the refcount reaches 0, the image
 * with the associated metadata and memory will be freed.
 */
static inline void
gst_egl_image_unref (GstEGLImage * image)
{
  gst_mini_object_unref (GST_MINI_OBJECT_CAST (image));
}

G_END_DECLS

#endif /* _GST_EGL_IMAGE_H_ */

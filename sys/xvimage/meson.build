xvimage_sources = [
  'xvcontext.c',
  'xvimage.c',
  'xvimageallocator.c',
  'xvimagepool.c',
  'xvimagesink.c',
]

no_warn_args = []
# XKeycodeToKeysym is deprecated, but we use it when Xkb is unavailable
if cc.has_argument ('-Wno-deprecated-declarations')
  no_warn_args += '-Wno-deprecated-declarations'
endif

libdrm_dep = dependency('libdrm')

xvideo_dep = dependency('xv', required : get_option('xvideo'))

if xvideo_dep.found()
  gstxvimage = library('gstxvimagesink',
    xvimage_sources,
    c_args : gst_plugins_base_args + no_warn_args,
    include_directories: [configinc, libsinc],
    dependencies : glib_deps + [video_dep, gst_base_dep, gst_dep, x11_dep, xshm_dep, xvideo_dep, libdrm_dep, libm, allocators_dep],
    install : true,
    install_dir : plugins_install_dir,
  )
  pkgconfig.generate(gstxvimage, install_dir : plugins_pkgconfig_install_dir)
  plugins += [gstxvimage]
endif

audio_inc = include_directories('.')

audio_headers = [
  'pipeline.h',
  'bgmpipeline.h',
  'effectpipeline.h'
]

audio_sources = [
  'pipeline.cpp',
  'bgmpipeline.cpp',
  'effectpipeline.cpp',
]

gst_version = '1.14.4'
version_arr = gst_version.split('.')
gst_version_major = version_arr[0].to_int()
gst_version_minor = version_arr[1].to_int()
gst_version_micro = version_arr[2].to_int()

gst_apiversion = '1.0'
soversion = 0

curversion = gst_version_minor * 100 + gst_version_micro
libversion = '@0@.@1@.0'.format(soversion, curversion)

glib_req = '>= 2.44.0'
gst_req = '>= @0@.@1@.0'.format(gst_version_major, gst_version_minor)

# mandatory gst deps
gst_dep = dependency('gstreamer-' + gst_apiversion, version : gst_req,
  fallback : ['gstreamer', 'gst_dep'])
gstbase_dep = dependency('gstreamer-base-' + gst_apiversion, version : gst_req,
  fallback : ['gstreamer', 'gst_base_dep'])

# declare dep
pipeline = shared_library('audiopipeline',
  audio_sources,
  include_directories : [audio_inc],
  version : libversion,
  soversion : soversion,
  install : true,
  dependencies : [gst_dep, gstbase_dep],
)

pipeline_dep = declare_dependency(link_with : pipeline,
  include_directories : [audio_inc],
  dependencies : [gst_dep, gstbase_dep],
  sources : audio_sources
)

project('stdx',
  languages: 'cpp',
  default_options: ['cpp_std=c++20'])

boost_dep = dependency('boost', required: true)

fmt_sub = subproject('fmt')
fmt_dep = fmt_sub.get_variable('fmt_dep')

intel_concurrency_sub = subproject('cpp-baremetal-concurrency')
intel_concurrency_dep = intel_concurrency_sub.get_variable('baremetal_concurrency_dep')

inc = include_directories('include')

stdx_dep = declare_dependency(
  include_directories: inc,
  dependencies: [boost_dep, fmt_dep, intel_concurrency_dep]
  )

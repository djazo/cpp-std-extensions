project('stdx',
  ['cpp'],
  default_options: ['cpp_std=c++20'])

boostmp11_sub = subproject('boostmp11')
boostmp11_dep = boostmp11_sub.get_variable('boostmp11_dep')

fmt_sub = subproject('fmt')
fmt_dep = fmt_sub.get_variable('fmt_dep')

baremetal_concurrency_sub = subproject('cpp-baremetal-concurrency')
baremetal_concurrency_dep = baremetal_concurrency_sub.get_variable('baremetal_concurrency_dep')

inc = include_directories('include')

stdx_dep = declare_dependency(
  include_directories: inc,
  dependencies: [boostmp11_dep, fmt_dep, baremetal_concurrency_dep]
  )

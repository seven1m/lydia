require 'rake'

desc "Build and run tests."
task :default => [:build, :test]

desc "Run all tests."
task :test do
  ruby "test/all.rb"
end

desc "Build parser C code using 'leg' binary (must have 'leg' in your PATH)."
task :leg do
  sh "leg ext/parser.leg -o ext/parser.c"
end

desc "Build C extension."
task :build do
  sh "ruby ext/extconf.rb && make"
end

# not used at this time"
task :bin => :build do
  require 'rbconfig'
  header_dir = Config::CONFIG['rubyhdrdir']
  arch_dir   = "#{header_dir}/#{Config::CONFIG['arch']}"
  lib_dir    = Config::CONFIG['libdir']
  ld_flags   = Config::CONFIG['LDFLAGS']
  libs       = Config::CONFIG['LIBS']
  lib_arg    = Config::CONFIG['LIBRUBYARG']
  sh "gcc ext/parser.c -o bin/airball_c -std=c99 -I #{header_dir} -I #{arch_dir} -L #{lib_dir} #{ld_flags} #{libs} #{lib_arg}"
end

task :stats do
  sh "sloccount ."
end

require 'rake'

task :default => [:test]

task :test do
  ruby "test/all.rb"
end

task :stats do
  exec "sloccount ."
end

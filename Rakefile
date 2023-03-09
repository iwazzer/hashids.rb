require "rake/testtask"
require "bundler/gem_tasks"

require "rake/extensiontask"

task :build => :compile

Rake::ExtensionTask.new("hashids_ext") do |ext|
  ext.lib_dir = "lib/hashids_ext"
end

Rake::TestTask.new(:spec) do |t|
  t.test_files = FileList['spec/**/*_spec.rb']
end

task :default => [:clobber, :compile, :spec]

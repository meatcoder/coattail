Gem::Specification.new do |s|
  s.name        = 'coattail'
  s.version     = '0.0.1'
  s.date        = '2017-08-14'
  s.summary     = "tail files as they update"
  s.description =  s.summary
  s.authors     = ["Sim Domingo"]
  s.email       = 'sim@github.com'
  s.files       = Dir.glob("ext/**/*.{c,rb}") + Dir.glob("lib/**/*.rb")
  s.homepage    =
    'http://rubygems.org/meatcoder/coattail'
  s.license       = 'MIT'
	s.extensions << "ext/coattail/extconf.rb"
	s.add_development_dependency "rake-compiler"
end

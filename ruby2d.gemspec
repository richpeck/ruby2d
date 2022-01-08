require_relative 'lib/ruby2d/version'

Gem::Specification.new do |s|
  s.name        = 'ruby2d-rpeck-windows'
  s.version     = Ruby2D::VERSION
  s.summary     = 'Ruby 2D'
  s.description = 'Make cross-platform 2D applications in Ruby (changed version RPECK 08/01/2022)'
  s.homepage    = 'http://www.ruby2d.com'
  s.license     = 'MIT'
  s.author      = 'Tom Black'
  s.email       = 'tom@blacktm.com'

  s.required_ruby_version = '>= 2.0.0'
  s.add_development_dependency 'rspec', '~> 3.8'

  s.files = Dir.glob('lib/**/*') +
            Dir.glob('assets/**/*') +
            Dir.glob('ext/**/*.{h,c,rb}')
  s.extensions = ['ext/ruby2d/extconf.rb']
  s.executables << 'ruby2d'
end

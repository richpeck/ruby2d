This is the Windows version of the Ruby2D gem I hacked to get the build pipeline to work.

I've written about how to get the system set up here: -
https://www.thefastlaneforum.com/community/threads/how-to-install-ruby2d-on-windows-game-programming.102392/


--

You need to add this gem from github, then execute any commands with "bundle exec" (unfortunately, the gem's executable is not present in the PATH if it's built from git):-
`gem 'ruby2d', github: 'richpeck/ruby2d', branch: 'windows', submodules: true`

Then use `bundle exec ruby2d build --native main.rb` to build or `bundle exec ruby x.rb` to run.

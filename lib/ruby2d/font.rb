# Ruby2D::Font

module Ruby2D
  class Font
    @@loaded_fonts = {}

    attr_reader :ttf_font

    def initialize(path, size, style=nil)
      @ttf_font = Font.ext_load(path, size, style.to_s)
    end

    class << self
      def load(path, size, style=nil)
        unless File.exist? path
          raise Error, "Cannot find font file `#{path}`"
        end

        @@loaded_fonts[[path, size, style]] ||= Font.new(path, size, style)
      end

      # List all fonts, names only
      def all
        all_paths.map { |path| path.split('/').last.chomp('.ttf').downcase }.uniq.sort
      end

      # Find a font file path from its name
      def path(font_name)
        font = all_paths.find { |path| path.downcase.include?(font_name) }
        font = File.join(directory, font) if directory.include?('Windows') && RUBY_ENGINE == 'mruby'
        font
      end

      # Get all fonts with full file paths
      def all_paths
        if RUBY_ENGINE == 'mruby'
          fonts = Dir.entries(directory)
        # If MRI and/or non-Bash shell (like cmd.exe)
        else
          fonts = Dir["#{directory}/**/*.ttf"]
        end

        fonts = fonts.reject do |f|
          f.downcase.include?('bold')    ||
          f.downcase.include?('italic')  ||
          f.downcase.include?('oblique') ||
          f.downcase.include?('narrow')  ||
          f.downcase.include?('black')
        end

        fonts.select {|font| File.extname(font) == ".ttf" }.sort_by { |f| f.downcase.chomp '.ttf' }
      end

      # Get the default font
      def default
        if all.include? 'arial'
          path 'arial'
        else
          all_paths.first 
        end
      end

      # Get the fonts directory for the current platform
      def directory
        macos_font_path   = '/Library/Fonts'
        linux_font_path   = '/usr/share/fonts'
        windows_font_path = 'C:/Windows/Fonts'
        openbsd_font_path = '/usr/X11R6/lib/X11/fonts'

        # If MRI and/or non-Bash shell (like cmd.exe)
        if Object.const_defined? :RUBY_PLATFORM
          case RUBY_PLATFORM
          when /darwin/  # macOS
            macos_font_path
          when /linux/
            linux_font_path
          when /mingw/
            windows_font_path
          when /openbsd/
            openbsd_font_path
          end
        # If MRuby
        else

          # RPECK 23/12/2021 -- the uname command was not available on Windows
          [macos_font_path, linux_font_path, windows_font_path, openbsd_font_path].each do |folder|
            return folder if File.exists? folder 
          end

        end
      end

    end

  end
end

#http://stackoverflow.com/questions/2885190/using-pythons-configparser-to-read-a-file-without-section-name
import ConfigParser
import StringIO

class SectionlessConfigParser(ConfigParser.RawConfigParser):
    """
    Extends ConfigParser to allow files without sections.

    This is done by wrapping read files and prepending them with a placeholder
    section, which defaults to '__config__'
    """

    def __init__(self, default_section=None, *args, **kwargs):
        ConfigParser.RawConfigParser.__init__(self, *args, **kwargs)

        self._default_section = None
        self.set_default_section(default_section or '__config__')

    def get_default_section(self):
        return self._default_section

    def set_default_section(self, section):
        self.add_section(section)

        # move all values from the previous default section to the new one
        try:
            default_section_items = self.items(self._default_section)
            self.remove_section(self._default_section)
        except ConfigParser.NoSectionError:
            pass
        else:
            for (key, value) in default_section_items:
                self.set(section, key, value)

        self._default_section = section

    def read(self, filenames):
        if isinstance(filenames, basestring):
            filenames = [filenames]

        read_ok = []
        for filename in filenames:
            try:
                with open(filename) as fp:
                    self.readfp(fp)
            except IOError:
                continue
            else:
                read_ok.append(filename)

        return read_ok

    def readfp(self, fp, *args, **kwargs):
        stream = StringIO.StringIO()

        try:
            stream.name = fp.name
        except AttributeError:
            pass

        stream.write('[' + self._default_section + ']\n')
        stream.write(fp.read())
        stream.seek(0, 0)

        return ConfigParser.RawConfigParser.readfp(self, stream, *args,
                                                   **kwargs)

    def write(self, fp):
        # Write the items from the default section manually and then remove them
        # from the data. They'll be re-added later.
        try:
            default_section_items = self.items(self._default_section)
            self.remove_section(self._default_section)

            for (key, value) in default_section_items:
                fp.write("{0} = {1}\n".format(key, value))

            fp.write("\n")
        except ConfigParser.NoSectionError:
            pass

        ConfigParser.RawConfigParser.write(self, fp)

        self.add_section(self._default_section)
        for (key, value) in default_section_items:
            self.set(self._default_section, key, value)

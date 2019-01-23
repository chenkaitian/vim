import os   
import ycm_core   
flags = [   
    '-std=c++11',   
    '-stdlib=libc++',   
    '-Wno-deprecated-declarations',   
    '-Wno-disabled-macro-expansion',   
    '-Wno-float-equal',   
    '-Wno-c++98-compat',   
    '-Wno-c++98-compat-pedantic',   
    '-Wno-global-constructors',   
    '-Wno-exit-time-destructors',   
    '-Wno-missing-prototypes',   
    '-Wno-padded',   
    '-x',   
    'c++',   
    '-isystem', 'C:/Program Files/vim/VimData/include/cplusplus/',
    '-isystem', 'C:/Program Files/vim/VimData/include/cplusplus/backward/',
    '-isystem', 'C:/Program Files/vim/VimData/include/cplusplus/bits/',
    '-isystem', 'C:/Program Files/vim/VimData/include/cplusplus/ext/',
    '-isystem', 'C:/Program Files/vim/VimData/include/cplusplus/parallel/',
    '-isystem', 'C:/Program Files/vim/VimData/include/cplusplus/tr1/',
    '-isystem', 'C:/Program Files/vim/VimData/include/cplusplus/tr1_impl/',
    '-isystem', 'C:/Program Files/vim/VimData/include/cplusplus/x86_64-redhat-linux/',
    '-isystem', 'C:/Program Files/vim/VimData/include/linux/',
    '-I', '.',   
    '-I', 'C:/Program Files/vim/VimData/include/taf/config/',
    '-I', 'C:/Program Files/vim/VimData/include/taf/hessian/',
    '-I', 'C:/Program Files/vim/VimData/include/taf/jce/',
    '-I', 'C:/Program Files/vim/VimData/include/taf/jmem/',
    '-I', 'C:/Program Files/vim/VimData/include/taf/log/',
    '-I', 'C:/Program Files/vim/VimData/include/taf/nodeF/',
    '-I', 'C:/Program Files/vim/VimData/include/taf/notify/',
    '-I', 'C:/Program Files/vim/VimData/include/taf/parse/',
    '-I', 'C:/Program Files/vim/VimData/include/taf/patch/',
    '-I', 'C:/Program Files/vim/VimData/include/taf/promise/',
    '-I', 'C:/Program Files/vim/VimData/include/taf/property/',
    '-I', 'C:/Program Files/vim/VimData/include/taf/servant',
    '-I', 'C:/Program Files/vim/VimData/include/taf/util',
]   
compilation_database_folder = ''   
if compilation_database_folder:   
  database = ycm_core.CompilationDatabase( compilation_database_folder )   
else:   
  database = None   
SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c', '.m', '.mm' ]   
def DirectoryOfThisScript():   
  return os.path.dirname( os.path.abspath( __file__ ) )   
def MakeRelativePathsInFlagsAbsolute( flags, working_directory ):   
  if not working_directory:   
    return list( flags )   
  new_flags = []   
  make_next_absolute = False   
  path_flags = [ '-isystem', '-I', '-iquote', '--sysroot=' ]   
  for flag in flags:   
    new_flag = flag   
    if make_next_absolute:   
      make_next_absolute = False   
      if not flag.startswith( '/' ):   
        new_flag = os.path.join( working_directory, flag )   
    for path_flag in path_flags:   
      if flag == path_flag:   
        make_next_absolute = True   
        break   
      if flag.startswith( path_flag ):   
        path = flag[ len( path_flag ): ]   
        new_flag = path_flag + os.path.join( working_directory, path )   
        break   
    if new_flag:   
      new_flags.append( new_flag )   
  return new_flags   
def IsHeaderFile( filename ):   
  extension = os.path.splitext( filename )[ 1 ]   
  return extension in [ '.h', '.hxx', '.hpp', '.hh' ]   
def GetCompilationInfoForFile( filename ):   
  if IsHeaderFile( filename ):   
    basename = os.path.splitext( filename )[ 0 ]   
    for extension in SOURCE_EXTENSIONS:   
      replacement_file = basename + extension   
      if os.path.exists( replacement_file ):   
        compilation_info = database.GetCompilationInfoForFile(          replacement_file )  
        if compilation_info.compiler_flags_:   
          return compilation_info   
    return None   
  return database.GetCompilationInfoForFile( filename )   
def FlagsForFile( filename, **kwargs ):   
  if database:   
    compilation_info = GetCompilationInfoForFile( filename )   
    if not compilation_info:   
      return None   
    final_flags = MakeRelativePathsInFlagsAbsolute(   
      compilation_info.compiler_flags_,   
      compilation_info.compiler_working_dir_ )   
  else:   
    relative_to = DirectoryOfThisScript()   
    final_flags = MakeRelativePathsInFlagsAbsolute( flags, relative_to )   
  return {   
    'flags': final_flags,   
    'do_cache': True   
  } 

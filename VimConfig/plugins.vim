""""""""""""""关于ctags
"智能补全ctags -R --c++-kinds=+p --fields=+iaS --extra=+q
let OmniCpp_DefaultNamespaces = ["std"]
let OmniCpp_GlobalScopeSearch = 1  " 0 or 1
let OmniCpp_NamespaceSearch = 1   " 0 ,  1 or 2
let OmniCpp_DisplayMode = 1
let OmniCpp_ShowScopeInAbbr = 0
let OmniCpp_ShowPrototypeInAbbr = 1
let OmniCpp_ShowAccess = 1
let OmniCpp_MayCompleteDot = 1
let OmniCpp_MayCompleteArrow = 1
let OmniCpp_MayCompleteScope = 1
let g:AutoComplPop_lgnoreCaseOption = 1
"au CursorMovedI,InsertLeave * if pumvisible() == 0|silent! pclose|endif
""""""""""""""关于ctags

""""""""""""""Quickfix的设置
"set makeprg=gcc\ -oa.out\ %
"autocmd FileType c,cpp  map <buffer> <leader><space> :w<cr>:make<cr>
"nmap <leader>cn :cn<cr>
"nmap <leader>cp :cp<cr>
"nmap <leader>cw :cw 10<cr>
""""""""""""""Quickfix的设置

""""""""""ShowMarks的设置
"高亮小写标记
let g:showmarks_hlline_lower = 1
let g:showmarks_hlline_upper = 1
let g:showmarks_ignore_type = "hqm"
let g:showmarks_enable=0
let g:showmarks_include = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
""""""""""ShowMarks的设置

""""""""""MarksBrowser的设置
let g:marksCloseWhenSelected = 1
nmap <silent> <leader>mk :MarksBrowser<CR>
""""""""""MarksBrowser的设置

""""""""""NERD_Tree的设置
nmap <silent> <leader>nn :NERDTreeToggle<CR>
let NERDChristmasTree=1
let NERDTreeBookmarksFile= g:VIM_PLUGIN_DATA_PATH . "/NerdTree/NerdBookmarks.txt"
"总是显示bookmarks
let NERDTreeShowBookmarks=1
let NERDTreeShowFiles=1
let NERDTreeShowHidden=1
"总是显示行号
let NERDTreeShowLineNumbers=1
let NERDTreeWinPos='left'
let NERDTreeWinSize=60
"打开文件后退出
let NERDTreeQuitOnOpen=1
"过滤文件，使用正则表达式
let NERDTreeIgnore=['\.pdf$','\.wmv','\.flv','\.o']
"""""""""""""""""""""""""
""""""""""SuperTab的设置
"so supertab.vma
let g:SuperTabDefaultCompletionType="<C-X><C-O>"
let g:SuperTabRetainCompletionType = 2
""""""""""SuperTab的设置

""""""""""MRU.vim的设置
let MRU_File = g:VIM_PLUGIN_DATA_PATH . "/MRU/MRU.txt"
let MRU_Max_Entries = 20
let MRU_Exclude_Files = '^/tmp/.*\|^/var/tmp/.*'
let MRU_Include_Files = '\.c$\|\.h$|\.cpp$\'
let MRU_Window_Height = 15
let MRU_Use_Current_Window = 1
let MRU_Auto_Close = 0
let MRU_Add_Menu = 0
let MRU_Max_Menu_Entries = 10
""""""""""MRU.vim的设置

""""""""""BufExplorer的设置
map <leader><leader> <leader>be
let g:bufExplorerDefaultHelp=0       " Do not show default help.
let g:bufExplorerShowRelativePath=1  " Show relative paths.
let g:bufExplorerSortBy='mru'        " Sort by most recently used.
let g:bufExplorerSplitRight=0        " Split left.
let g:bufExplorerSplitVertical=1     " Split vertically.
let g:bufExplorerSplitVertSize=30    " Split width
let g:bufExplorerUseCurrentWindow=1  " Open in new window.
let g:bufExplorerDetailedHelp=1
autocmd BufWinEnter \[Buf\ List\] setl nonumber
""""""""""BufExplorer的设置

""""""""""minibufexpl的设置
"let g:miniBufExplMapWindowNavArrows = 1
"let g:miniBufExplModSelTarget = 1
"实现<C-Tab>     向前循环切换到每个buffer上,并在当前窗口打开
"let g:miniBufExplMapCTabSwitchBufs = 1
"<C-S-Tab>     向后循环切换到每个buffer上,并在当前窗口打开
"可以用<C-h,j,k,l>切换到上下左右的窗口中去
let g:miniBufExplMapWindowNavVim = 1
let g:miniBufExplMapWindowNavArrows = 1
let g:miniBufExplMapCTabSwitchBufs = 1
let g:miniBufExplModSelTarget = 1
"let g:miniBufExplMoreThanOne=0
let g:miniBufExplorerMoreThanOne=0
let g:miniBufExplorerAutoUpdate=1
let g:minibufexplEnable=1
""""""""""minibufexpl的设置

""""""""""Tlist的设置
let g:Tlist_Use_Right_Window=0
let g:Tlist_Show_One_File=1
let g:Tlist_Show_Menu = 1
let g:Tlist_Compact_Format=1
"当只有Tlist窗口时自动退出
let g:Tlist_Exit_OnlyWindow=1
let g:Tlist_Sort_Type='name'
let g:Tlist_Auto_Open=1
let g:Tlist_File_fold_Auto_Close=1
let g:Tlist_WinWidth=50
"设置系统tags的程序
if has("win32")
	let g:Tlist_Ctags_Cmd='ctags.exe'
else
	let g:Tlist_Ctags_Cmd='ctags'
endif
if has("unix")
	set tags=/usr/include/tags,../tags
	set tags+=./tags
endif
"nmap <F11> <Esc>:!ctags -R <CR>
nmap <F11> <Esc>:!rm -f tags && ctags -R --c++-kinds=+p --fields=+iaS --extra=+q <CR>
nnoremap <silent> <leader>tl :TlistToggle<cr>
""""""""""Tlist的设置

""""""""""lookupfile的设置
"最少输入2个字符才开始查找
let g:LookupFile_MinPatLength = 2
"不保存上次查找的字符串
let g:LookupFile_PreserveLastPattern = 0
"保存查找历史
let g:LookupFile_PreservePatternHistory = 1
"回车打开第一个匹配项目
let g:LookupFile_AlwaysAcceptFirst = 1
"不允许创建不存在的文件
let g:LookupFile_AllowNewFiles = 0
"设置tag文件的名字
if filereadable("./filenametags")
let g:LookupFile_TagExpr = '"./filenametags"'
endif
"映射LookupFile为,lt
nmap <silent> <leader>lt :LUTags<cr>
"映射LUBufs为,lb
nmap <silent> <leader>lb :LUBufs<cr>
"映射LUWalk为,lw
nmap <silent> <leader>lw :LUWalk<cr>
" lookupfile查找文件时（不包括缓冲区和目录）忽略大小写
function! LookupFile_IgnoreCaseFunc(pattern)
    let _tags = &tags
    try
        let &tags = eval(g:LookupFile_TagExpr)
        let newpattern = '\c' . a:pattern
        let tags = taglist(newpattern)
    catch
        echohl ErrorMsg | echo "Exception: " . v:exception | echohl NONE
        return ""
    finally
        let &tags = _tags
    endtry

    " Show the matches for what is typed so far.
    let files = map(tags, 'v:val["filename"]')
    return files
endfunction
let g:LookupFile_LookupFunc = 'LookupFile_IgnoreCaseFunc'
""""""""""lookupfile的设置

""""""""""Mark的设置
nmap <silent> <leader>hl <Plug>MarkSet
vmap <silent> <leader>hl <Plug>MarkSet
nmap <silent> <leader>hh <Plug>MarkClear
vmap <silent> <leader>hh <Plug>MarkClear
nmap <silent> <leader>hr <Plug>MarkRegex
vmap <silent> <leader>hr <Plug>MarkRegex
""""""""""Mark的设置

""""""""""cscope的设置
set cscopequickfix=s-,c-,d-,i-,t-,e-
"一开始保存原来的目录，如果前面有设置autochdir,会导致编辑的时候目录也跟着变，到时候生成的
"tags和cscope文件会在那个编辑的文件的目录下，应该是在一开始的目录才对
let g:firstEnterDir = getcwd()
"获取要生成tags文件的当前目录(就是一开始进入的目录)后，就能够自动切换目录
set autochdir
"加载cscope.out，搜索上级目录
function! LoadCscopeOut()
	"g:IsLoadCscopeOut标记是否已经加载了cscope.out文件
	if exists('g:IsLoadCscopeOut')
		return 0
	endif
	if has("cscope")
        "调用之前保存当前目录
		"set csprg=/usr/bin/cscope
		set csto=0
		set cst
		set csverb
		set cspc=3
		"add any database in current dir
		if filereadable("cscope.out")
			:silent cs add cscope.out
		"else search cscope.out elsewhere
		else
		   let cscope_file=findfile("cscope.out", ".;")
		   let cscope_pre=matchstr(cscope_file, ".*/")
		   if !empty(cscope_file) && filereadable(cscope_file)
			   :silent exe "cs add" cscope_file cscope_pre
			   let g:IsLoadCscopeOut = 1
		   endif
		 endif
	endif
endfunction
"进入C、CPP文件的时候才加载cscope.out
autocmd FileType c,cpp call LoadCscopeOut()
nmap <C-H>s :cs find s <C-R>=expand("<cword>")<CR><CR>
nmap <C-H>g :cs find g <C-R>=expand("<cword>")<CR><CR>
nmap <C-H>c :cs find c <C-R>=expand("<cword>")<CR><CR>
nmap <C-H>t :cs find t <C-R>=expand("<cword>")<CR><CR>
nmap <C-H>e :cs find e <C-R>=expand("<cword>")<CR><CR>
nmap <C-H>f :cs find f <C-R>=expand("<cfile>")<CR><CR>
nmap <C-H>i :cs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
nmap <C-H>d :cs find d <C-R>=expand("<cword>")<CR><CR>
"执行系统命令更新ctags和cscope的相关文件
function! UpdateCtagsAndCscope()
	let g:curEnterDir = getcwd()
	"在这里不能使用‘cd g:firstEnterDir’，并不会起作用
	exec ":cd " . g:firstEnterDir
	"这里在后台执行这些命令，就不用等很久，并且要不断的按回车
	"在:后面加silent可以不需要按回车，但是从shell返回会导致黑屏，不知道为什么
	"注意 VIM 中是语句开头的 \ 代表连接上一句，不是结尾的 \
	exec ":!COUNT=`ps aux | grep -E '(cscope|ctags)' | grep -v grep | wc -l`; if [ $COUNT -lt 1 ]; then find . -name '*.h' "
		 \"-o -name '*.c' -o -name '*.cpp' -o -name '*.proto' > cscope.files && cscope -bkq -i cscope.files && "
		 \"ctags -R --c++-kinds=+p --fields=+iaS --extra=+q; fi &"
	exec ":cd " . g:curEnterDir
endfunction
nmap <C-L><C-L> :call UpdateCtagsAndCscope()<CR>
""""""""""cscope的设置

""""""""""c-support的设置
let g:alternateNoDefaultAlternate = 1
"6.0版本以后速度很慢，使用这个可以变快
let g:C_CreateMenusDelayed = 'yes'
""""""""""c-support的设置

""""""""""Tree explorer的设置
let g:treeExplVertical=1
let g:treeExplWinSize=30
""""""""""Tree explorer的设置

""""""""""BracketsComplement的设置
"au BufNewFile,BufRead *.c,*.cpp,*.h exec "source " . g:VIM_CONF_PATH . "/ftplugin/BracketsComplement.vim"
"au BufNewFile,BufRead *.txt exec "source " . g:VIM_CONF_PATH . "/ftplugin/text_Chinese_format.vim"
""""""""""BracketsComplement的设置

""""""""""(SrcExpl的设置
"设置SrcExpl一开始的高度
let g:SrcExpl_winHeight = 5
"设置SrcExpl的刷新时间，单位为毫秒
let g:SrcExpl_refreshTime = 300
"需要增加其他buffer的名字，避免冲突，使用命令":buffers!"查看
let g:SrcExpl_pluginList = ["__Tag_List__", "_NERD_tree_", "*unite*"]
"SrcExpl是否搜索本地定义(帮助文件中不建议开启)
let SrcExpl_searchLocalDef = 1
"设置SrcExpl打开时不自动更新
let SrcExpl_isUpdateTags = 0
"tags文件的更新命令
let SrcExpl_updateTagsCmd = "ctags --sort=foldcase -R ."
"设置在SrcExpl窗口中跳转到定义处的快捷键
let g:SrcExpl_jumpKey = "<ENTER>"
"设置在条状后回到原来的位置的快捷键
let g:SrcExpl_gobackKey = "<SPACE>"
"设置SrcExpl更新的快捷键
let g:SrcExpl_updateTagsKey = "<F12>"
""""""""""SrcExpl的设置)

""""""""""(indentLine的设置
let g:indentLine_enabled = 1
":IndentLinesToggle
""""""""""indentLine的设置)

""""""""""(ultisnips的设置
let g:UltiSnipsExpandTrigger="<c-k><c-k>"
let g:UltiSnipsListSnippets="<c-k><c-l>"
let g:UltiSnipsJumpForwardTrigger="<c-b>"
let g:UltiSnipsJumpBackwardTrigger="<c-z>"
let g:UltiSnipsUsePythonVersion = 2
""""""""""ultisnips的设置)

""""""""""(ale的设置
"ale
"始终开启标志列
let g:ale_sign_column_always = 1
let g:ale_set_highlights = 0
"自定义error和warning图标
let g:ale_sign_error = 'X'
let g:ale_sign_warning = '?'
"在vim自带的状态栏中整合ale
let g:ale_statusline_format = ['X %d', '? %d', 'OK']
"显示Linter名称,出错或警告等相关信息
let g:ale_echo_msg_error_str = 'E'
let g:ale_echo_msg_warning_str = 'W'
let g:ale_echo_msg_format = '[%linter%] %s [%severity%]'
"普通模式下，sp前往上一个错误或警告，sn前往下一个错误或警告
nmap sp <Plug>(ale_previous_wrap)
nmap sn <Plug>(ale_next_wrap)
"<Leader>s触发/关闭语法检查
nmap <Leader>et :ALEToggle<CR>
"<Leader>d查看错误或警告的详细信息
nmap <Leader>ed :ALEDetail<CR>
"文件内容发生变化时不进行检查
let g:ale_lint_on_text_changed = 'never'
"打开文件时不进行检查
let g:ale_lint_on_enter = 1
"对C/C++使用Clang进行语法检查
"let g:ale_linters = {'c': 'clang' , 'cpp': 'clang'}
let g:ale_linters = {'cpp': 'clang++'}
let g:ale_cpp_clang_options = '-std=c++11 -Wall'

if exists('g:loaded_ale_dont_use_this_in_other_plugins_please')
    set statusline+=\ [%{ALEGetStatusLine()}]
endif
""""""""""ale的设置)

""""""""""(vim-gutentags的设置
" gutentags 搜索工程目录的标志，碰到这些文件/目录名就停止向上一级目录递归
let g:gutentags_project_root = ['.svn', '.git', '.hg', '.project']
" 所生成的数据文件的名称
let g:gutentags_ctags_tagfile = '.tags'
" 将自动生成的 tags 文件全部放入 ~/.cache/tags 目录中，避免污染工程目录
let s:vim_tags = expand('~/.cache/tags')
let g:gutentags_cache_dir = s:vim_tags
" 配置 ctags 的参数
let g:gutentags_ctags_extra_args = ['--fields=+niazS', '--extras=+q']
let g:gutentags_ctags_extra_args += ['--c++-kinds=+px']
let g:gutentags_ctags_extra_args += ['--c-kinds=+px']
" 检测 ~/.cache/tags 不存在就新建
if !isdirectory(s:vim_tags)
   silent! call mkdir(s:vim_tags, 'p')
endif
""""""""""vim-gutentags的设置)

"source neocomplete.vim
"source syntastic.vim
source $VIM/VimConfig/YouCompleteme.vim

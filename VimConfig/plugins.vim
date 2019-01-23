""""""""""""""����ctags
"���ܲ�ȫctags -R --c++-kinds=+p --fields=+iaS --extra=+q
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
""""""""""""""����ctags

""""""""""""""Quickfix������
"set makeprg=gcc\ -oa.out\ %
"autocmd FileType c,cpp  map <buffer> <leader><space> :w<cr>:make<cr>
"nmap <leader>cn :cn<cr>
"nmap <leader>cp :cp<cr>
"nmap <leader>cw :cw 10<cr>
""""""""""""""Quickfix������

""""""""""ShowMarks������
"����Сд���
let g:showmarks_hlline_lower = 1
let g:showmarks_hlline_upper = 1
let g:showmarks_ignore_type = "hqm"
let g:showmarks_enable=0
let g:showmarks_include = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
""""""""""ShowMarks������

""""""""""MarksBrowser������
let g:marksCloseWhenSelected = 1
nmap <silent> <leader>mk :MarksBrowser<CR>
""""""""""MarksBrowser������

""""""""""NERD_Tree������
nmap <silent> <leader>nn :NERDTreeToggle<CR>
let NERDChristmasTree=1
let NERDTreeBookmarksFile= g:VIM_PLUGIN_DATA_PATH . "/NerdTree/NerdBookmarks.txt"
"������ʾbookmarks
let NERDTreeShowBookmarks=1
let NERDTreeShowFiles=1
let NERDTreeShowHidden=1
"������ʾ�к�
let NERDTreeShowLineNumbers=1
let NERDTreeWinPos='left'
let NERDTreeWinSize=60
"���ļ����˳�
let NERDTreeQuitOnOpen=1
"�����ļ���ʹ��������ʽ
let NERDTreeIgnore=['\.pdf$','\.wmv','\.flv','\.o']
"""""""""""""""""""""""""
""""""""""SuperTab������
"so supertab.vma
let g:SuperTabDefaultCompletionType="<C-X><C-O>"
let g:SuperTabRetainCompletionType = 2
""""""""""SuperTab������

""""""""""MRU.vim������
let MRU_File = g:VIM_PLUGIN_DATA_PATH . "/MRU/MRU.txt"
let MRU_Max_Entries = 20
let MRU_Exclude_Files = '^/tmp/.*\|^/var/tmp/.*'
let MRU_Include_Files = '\.c$\|\.h$|\.cpp$\'
let MRU_Window_Height = 15
let MRU_Use_Current_Window = 1
let MRU_Auto_Close = 0
let MRU_Add_Menu = 0
let MRU_Max_Menu_Entries = 10
""""""""""MRU.vim������

""""""""""BufExplorer������
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
""""""""""BufExplorer������

""""""""""minibufexpl������
"let g:miniBufExplMapWindowNavArrows = 1
"let g:miniBufExplModSelTarget = 1
"ʵ��<C-Tab>     ��ǰѭ���л���ÿ��buffer��,���ڵ�ǰ���ڴ�
"let g:miniBufExplMapCTabSwitchBufs = 1
"<C-S-Tab>     ���ѭ���л���ÿ��buffer��,���ڵ�ǰ���ڴ�
"������<C-h,j,k,l>�л����������ҵĴ�����ȥ
let g:miniBufExplMapWindowNavVim = 1
let g:miniBufExplMapWindowNavArrows = 1
let g:miniBufExplMapCTabSwitchBufs = 1
let g:miniBufExplModSelTarget = 1
"let g:miniBufExplMoreThanOne=0
let g:miniBufExplorerMoreThanOne=0
let g:miniBufExplorerAutoUpdate=1
let g:minibufexplEnable=1
""""""""""minibufexpl������

""""""""""Tlist������
let g:Tlist_Use_Right_Window=0
let g:Tlist_Show_One_File=1
let g:Tlist_Show_Menu = 1
let g:Tlist_Compact_Format=1
"��ֻ��Tlist����ʱ�Զ��˳�
let g:Tlist_Exit_OnlyWindow=1
let g:Tlist_Sort_Type='name'
let g:Tlist_Auto_Open=1
let g:Tlist_File_fold_Auto_Close=1
let g:Tlist_WinWidth=50
"����ϵͳtags�ĳ���
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
""""""""""Tlist������

""""""""""lookupfile������
"��������2���ַ��ſ�ʼ����
let g:LookupFile_MinPatLength = 2
"�������ϴβ��ҵ��ַ���
let g:LookupFile_PreserveLastPattern = 0
"���������ʷ
let g:LookupFile_PreservePatternHistory = 1
"�س��򿪵�һ��ƥ����Ŀ
let g:LookupFile_AlwaysAcceptFirst = 1
"�������������ڵ��ļ�
let g:LookupFile_AllowNewFiles = 0
"����tag�ļ�������
if filereadable("./filenametags")
let g:LookupFile_TagExpr = '"./filenametags"'
endif
"ӳ��LookupFileΪ,lt
nmap <silent> <leader>lt :LUTags<cr>
"ӳ��LUBufsΪ,lb
nmap <silent> <leader>lb :LUBufs<cr>
"ӳ��LUWalkΪ,lw
nmap <silent> <leader>lw :LUWalk<cr>
" lookupfile�����ļ�ʱ����������������Ŀ¼�����Դ�Сд
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
""""""""""lookupfile������

""""""""""Mark������
nmap <silent> <leader>hl <Plug>MarkSet
vmap <silent> <leader>hl <Plug>MarkSet
nmap <silent> <leader>hh <Plug>MarkClear
vmap <silent> <leader>hh <Plug>MarkClear
nmap <silent> <leader>hr <Plug>MarkRegex
vmap <silent> <leader>hr <Plug>MarkRegex
""""""""""Mark������

""""""""""cscope������
set cscopequickfix=s-,c-,d-,i-,t-,e-
"һ��ʼ����ԭ����Ŀ¼�����ǰ��������autochdir,�ᵼ�±༭��ʱ��Ŀ¼Ҳ���ű䣬��ʱ�����ɵ�
"tags��cscope�ļ������Ǹ��༭���ļ���Ŀ¼�£�Ӧ������һ��ʼ��Ŀ¼�Ŷ�
let g:firstEnterDir = getcwd()
"��ȡҪ����tags�ļ��ĵ�ǰĿ¼(����һ��ʼ�����Ŀ¼)�󣬾��ܹ��Զ��л�Ŀ¼
set autochdir
"����cscope.out�������ϼ�Ŀ¼
function! LoadCscopeOut()
	"g:IsLoadCscopeOut����Ƿ��Ѿ�������cscope.out�ļ�
	if exists('g:IsLoadCscopeOut')
		return 0
	endif
	if has("cscope")
        "����֮ǰ���浱ǰĿ¼
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
"����C��CPP�ļ���ʱ��ż���cscope.out
autocmd FileType c,cpp call LoadCscopeOut()
nmap <C-H>s :cs find s <C-R>=expand("<cword>")<CR><CR>
nmap <C-H>g :cs find g <C-R>=expand("<cword>")<CR><CR>
nmap <C-H>c :cs find c <C-R>=expand("<cword>")<CR><CR>
nmap <C-H>t :cs find t <C-R>=expand("<cword>")<CR><CR>
nmap <C-H>e :cs find e <C-R>=expand("<cword>")<CR><CR>
nmap <C-H>f :cs find f <C-R>=expand("<cfile>")<CR><CR>
nmap <C-H>i :cs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
nmap <C-H>d :cs find d <C-R>=expand("<cword>")<CR><CR>
"ִ��ϵͳ�������ctags��cscope������ļ�
function! UpdateCtagsAndCscope()
	let g:curEnterDir = getcwd()
	"�����ﲻ��ʹ�á�cd g:firstEnterDir����������������
	exec ":cd " . g:firstEnterDir
	"�����ں�ִ̨����Щ����Ͳ��õȺܾã�����Ҫ���ϵİ��س�
	"��:�����silent���Բ���Ҫ���س������Ǵ�shell���ػᵼ�º�������֪��Ϊʲô
	"ע�� VIM ������俪ͷ�� \ ����������һ�䣬���ǽ�β�� \
	exec ":!COUNT=`ps aux | grep -E '(cscope|ctags)' | grep -v grep | wc -l`; if [ $COUNT -lt 1 ]; then find . -name '*.h' "
		 \"-o -name '*.c' -o -name '*.cpp' -o -name '*.proto' > cscope.files && cscope -bkq -i cscope.files && "
		 \"ctags -R --c++-kinds=+p --fields=+iaS --extra=+q; fi &"
	exec ":cd " . g:curEnterDir
endfunction
nmap <C-L><C-L> :call UpdateCtagsAndCscope()<CR>
""""""""""cscope������

""""""""""c-support������
let g:alternateNoDefaultAlternate = 1
"6.0�汾�Ժ��ٶȺ�����ʹ��������Ա��
let g:C_CreateMenusDelayed = 'yes'
""""""""""c-support������

""""""""""Tree explorer������
let g:treeExplVertical=1
let g:treeExplWinSize=30
""""""""""Tree explorer������

""""""""""BracketsComplement������
"au BufNewFile,BufRead *.c,*.cpp,*.h exec "source " . g:VIM_CONF_PATH . "/ftplugin/BracketsComplement.vim"
"au BufNewFile,BufRead *.txt exec "source " . g:VIM_CONF_PATH . "/ftplugin/text_Chinese_format.vim"
""""""""""BracketsComplement������

""""""""""(SrcExpl������
"����SrcExplһ��ʼ�ĸ߶�
let g:SrcExpl_winHeight = 5
"����SrcExpl��ˢ��ʱ�䣬��λΪ����
let g:SrcExpl_refreshTime = 300
"��Ҫ��������buffer�����֣������ͻ��ʹ������":buffers!"�鿴
let g:SrcExpl_pluginList = ["__Tag_List__", "_NERD_tree_", "*unite*"]
"SrcExpl�Ƿ��������ض���(�����ļ��в����鿪��)
let SrcExpl_searchLocalDef = 1
"����SrcExpl��ʱ���Զ�����
let SrcExpl_isUpdateTags = 0
"tags�ļ��ĸ�������
let SrcExpl_updateTagsCmd = "ctags --sort=foldcase -R ."
"������SrcExpl��������ת�����崦�Ŀ�ݼ�
let g:SrcExpl_jumpKey = "<ENTER>"
"��������״��ص�ԭ����λ�õĿ�ݼ�
let g:SrcExpl_gobackKey = "<SPACE>"
"����SrcExpl���µĿ�ݼ�
let g:SrcExpl_updateTagsKey = "<F12>"
""""""""""SrcExpl������)

""""""""""(indentLine������
let g:indentLine_enabled = 1
":IndentLinesToggle
""""""""""indentLine������)

""""""""""(ultisnips������
let g:UltiSnipsExpandTrigger="<c-k><c-k>"
let g:UltiSnipsListSnippets="<c-k><c-l>"
let g:UltiSnipsJumpForwardTrigger="<c-b>"
let g:UltiSnipsJumpBackwardTrigger="<c-z>"
let g:UltiSnipsUsePythonVersion = 2
""""""""""ultisnips������)

""""""""""(ale������
"ale
"ʼ�տ�����־��
let g:ale_sign_column_always = 1
let g:ale_set_highlights = 0
"�Զ���error��warningͼ��
let g:ale_sign_error = 'X'
let g:ale_sign_warning = '?'
"��vim�Դ���״̬��������ale
let g:ale_statusline_format = ['X %d', '? %d', 'OK']
"��ʾLinter����,����򾯸�������Ϣ
let g:ale_echo_msg_error_str = 'E'
let g:ale_echo_msg_warning_str = 'W'
let g:ale_echo_msg_format = '[%linter%] %s [%severity%]'
"��ͨģʽ�£�spǰ����һ������򾯸棬snǰ����һ������򾯸�
nmap sp <Plug>(ale_previous_wrap)
nmap sn <Plug>(ale_next_wrap)
"<Leader>s����/�ر��﷨���
nmap <Leader>et :ALEToggle<CR>
"<Leader>d�鿴����򾯸����ϸ��Ϣ
nmap <Leader>ed :ALEDetail<CR>
"�ļ����ݷ����仯ʱ�����м��
let g:ale_lint_on_text_changed = 'never'
"���ļ�ʱ�����м��
let g:ale_lint_on_enter = 1
"��C/C++ʹ��Clang�����﷨���
"let g:ale_linters = {'c': 'clang' , 'cpp': 'clang'}
let g:ale_linters = {'cpp': 'clang++'}
let g:ale_cpp_clang_options = '-std=c++11 -Wall'

if exists('g:loaded_ale_dont_use_this_in_other_plugins_please')
    set statusline+=\ [%{ALEGetStatusLine()}]
endif
""""""""""ale������)

""""""""""(vim-gutentags������
" gutentags ��������Ŀ¼�ı�־��������Щ�ļ�/Ŀ¼����ֹͣ����һ��Ŀ¼�ݹ�
let g:gutentags_project_root = ['.svn', '.git', '.hg', '.project']
" �����ɵ������ļ�������
let g:gutentags_ctags_tagfile = '.tags'
" ���Զ����ɵ� tags �ļ�ȫ������ ~/.cache/tags Ŀ¼�У�������Ⱦ����Ŀ¼
let s:vim_tags = expand('~/.cache/tags')
let g:gutentags_cache_dir = s:vim_tags
" ���� ctags �Ĳ���
let g:gutentags_ctags_extra_args = ['--fields=+niazS', '--extras=+q']
let g:gutentags_ctags_extra_args += ['--c++-kinds=+px']
let g:gutentags_ctags_extra_args += ['--c-kinds=+px']
" ��� ~/.cache/tags �����ھ��½�
if !isdirectory(s:vim_tags)
   silent! call mkdir(s:vim_tags, 'p')
endif
""""""""""vim-gutentags������)

"source neocomplete.vim
"source syntastic.vim
source $VIM/VimConfig/YouCompleteme.vim

""""""""""语言问题，包括编码、乱码问题等
"set guifontwide="Microsoft YaHei":h12
"设置VIM的行数和列数
"set lines=50 columns=200
"设置文字的最大长度
set textwidth=250
"colo peachpuff
"source $VIMRUNTIME/vimrc_example.vim
"source $VIMRUNTIME/mswin.vim
"behave mswin
"设置VIM所使用的编码方式
set encoding=utf-8
"菜单所使用的语言翻译
set langmenu=zh_CN.UTF-8
"设置文件的编码
set fileencodings=ucs-bom,utf-8,cp936,gb18030,big5,latin1
"set fencs=utf-8,usc-bom,euc-jp,gb18030,gbk,gb2312,cp936
language message zh_CN.UTF-8
if has("win32")
	set fileencoding=chinese
	"在gui模式下，设置窗口初始位置
	"winpos 235 235
	"在GUI下窗口最大化
	set winaltkeys=no
	au GUIEnter * simalt ~x
	"在GUI模式下光标不闪烁
	set gcr=a:blinkon0
	"set gcr=a:block-blinkon0
else
	set fileencoding=utf-8
endif
let &termencoding=&encoding
source $VIMRUNTIME/delmenu.vim
source $VIMRUNTIME/menu.vim
""""""""""语言问题，包括编码、乱码问题等

""""""""""自定义变量
if has("win32")
	"vimrc的位置
	"注意这里的$VIMRUNTIME最好不要写在双引号里
	let g:VIMRC_PATH = $VIMRUNTIME . "/../_vimrc"
	"vim个人配置文件的位置
	let g:VIM_DATA_PATH = $VIM . "/VimData"
else
	let g:VIMRC_PATH = $HOME . "/.vimrc"
	let g:VIM_DATA_PATH = $HOME . "/.vim/VimData"
endif
"插件的数据路径
let g:VIM_PLUGIN_DATA_PATH = g:VIM_DATA_PATH . "/plugins"
""""""""""自定义变量

""""""""""""""一些系统设置
"set guifont=Consolas\ 13
set guifont=Consolas:h11
if &term=="xterm"
	"number of colors
	set t_Co=256
endif
"颜色方案
"let g:solarized_termcolors=256
"set background=dark
"colorscheme solarized
let g:molokai_original = 1
let g:rehash256 = 1
colorscheme molokai
"设置<leader>
let mapleader=","
"光标在窗口上下边界时距离边界6行即开始滚屏
set so=6  "set scrolloff
"显示行号
set number
"突出显示当前行
set cursorline
 "设置查找系统运行时的目录
if has("unix")
	"set runtimepath += ~/.vim/textmateOnly
	"set runtimepath+=/home/hunch/.vim/after
endif
"使用双字节的ascii码
set ambiwidth=double
"自动重新加载外部修改内容(这个文件还没有被vim修改)
set autoread
"设置当使用命令':first'、':last'等得时候，自动写入文件，但是不会对':exit',':quit'等命令起效"
"set autowrite
"不自动切换当前目录为当前文件所在的目录,为了配合后面的cscope生成tags、cscope文件等
set noautochdir
"设置有错误信息时不发出beep声音
"no bell settings {{{
set noerrorbells
set visualbell
if has('autocmd')
  autocmd guienter * set vb t_vb=
endif
"no bell settings end }}}
"在编辑过程中，在右下角显示光标位置的状态行
"这里应该修改为主编辑窗口才显示,其他窗口不显示
set ruler
set rulerformat=%20(%2*%<%f%=\ %m%r\ %3l\ %c\ %p%%%)
"启动的时候不显示那个援助儿童的提示
set shortmess=atI
"自动换行显示
set wrap
"设定命令行的行数为1
set cmdheight=1
"显示状态栏 (默认值为1,无法显示状态栏)
set laststatus=2
"设置状态栏显示的内容
set statusline=%f%m%r%h%w\ [format=%{&ff}]\ [type=%y]\ [ascii=\%03.3b]\ [hex=\%02.2b]\ [pos=%04l,%04v][%p%%]\ [len=%l]
"当键入一个括号时，如果有匹配的，则短暂显示另一半括号
set showmatch
"选项:当一个文件存在时,对其进行覆写之前,会进行一份源文件的备份,写入完成后删除备份"
set writebackup
"类似nowritebackup,但是写入完成后备份会被保留
set nobackup
"在insert模式下能用删除键进行删除
set backspace=indent,eol,start
"增强模式中的命令行自动完成操作(例如按tab键会自动完成命令)
set wildmenu
"搜索的时候不区分大小写
set ignorecase
"在ignorecase为on时，如果存在一个大写字母，则自动关闭ignorecase，不适用于"*","#","gd"搜索
set smartcase
"在进行向前向后搜索时，高亮所有符合模式的关键字
set hlsearch
"在命令行下键入字符时，高亮符合的关键字
set incsearch
"不在单词中间断行
set linebreak
"显示右下角普通模式下的输入状态
set showcmd
"设置折叠方案,设置成syntax会有问题
"set fdm=indent
"单词移动时定义什么字母和符号属于一个单词
set iskeyword=_,$,@,%,#,48-57,192-255
"设置帮助语言
if version >= 603
  set helplang=cn
endif
"打开自己工程的时候自动加载的脚本
if getfsize(".vimscript") > 0
  source .vimscript
endif

:runtime! ftplugin/man.vim
"帮助文件的命令，当在普通模式下键入'K'时，会调用外部程序进行查找
set keywordprg=dicty
"不使用交换文件
set noswapfile

"退出 vim 后，仍然可以执行 undo 操作
try
    "set undodir=$VIM/VimData/undo
    "set undofile
catch

endtry
""""""""""""""一些系统设置

""""""""""""""语法、文件类型
"如果是C、CPP、VIM文件，则自动移除行尾的空格
au BufWritePre * call RemoveTrailingSpace()
"移除行尾的空格
function! RemoveTrailingSpace()
  if $VIM_HATE_SPACE_ERRORS != '0' &&
        \(&filetype == 'c' || &filetype == 'cpp' || &filetype == 'vim')
    normal m`
    silent! :%s/\s\+$//e
    normal ``
  endif
endfunction
"proto
augroup filetype
    au! BufRead,BufNewFile *.proto setfiletype proto
augroup end
""""""""""""""语法、文件类型

""""""""""""""缩进、补全设置
"设置补全时使用的方式(如menu为弹出菜单)
set completeopt=longest,menu
" 继承前一行的缩进方式，特别适用于多行注释
set autoindent
"为C程序提供自动缩进
"set smartindent
" 使用C样式的缩进
set cindent
" 自动缩进每一步使用的空格数目
set shiftwidth=4
" tab 键代表的空格数
set tabstop=4
" 敲入 tab 键时实际占有的列数
set softtabstop=4
" 插入 tab 键时，使用合适数量的空格代替
set expandtab

au FileType c,cpp setlocal cinoptions=:0,g0,(0,w1 shiftwidth=4 tabstop=4 softtabstop=4
au FileType vim setlocal cinoptions=:0,g0,(0,w1 shiftwidth=4 tabstop=4 softtabstop=4
au FileType diff setlocal shiftwidth=4 tabstop=4
au FileType html setlocal autoindent indentexpr=
au FileType changelog setlocal textwidth=146
au FileType text setlocal textwidth=100 tabstop=4 shiftwidth=4 formatoptions+=mM autoindent
if has("unix")
	au BufEnter /usr/* call GnuIndent()
endif
function! GnuIndent()
  setlocal cinoptions=>4,n-2,{2,^-2,:2,=2,g0,h2,p5,t0,+2,(0,u0,w1,m1
  setlocal shiftwidth=2
  setlocal tabstop=8
endfunction

if !exists('g:AutoComplPop_Behavior')
    let g:AutoComplPop_Behavior = {}
    let g:AutoComplPop_Behavior['php'] = []
    call add(g:AutoComplPop_Behavior['php'], {
            \   'command'   : "\<C-x>\<C-o>",
            \   'pattern'   : printf('\(->\|::\|\$\)\k\{%d,}$', 0),
            \   'repeat'    : 0,
            \})
endif
"补全文件名时忽略的文件
set suffixes=.bak,~,.o,.info,.swp,.aux,.bbl,.blg,.dvi,.lof,.log,.log,.ps,.toc
"自动替换的补全
ab #i #include
ab #d #define
ab #ssh #!/bin/bash<CR>
"输入pra自动补全printf,并且将光标定位到\n之前w如果要继续输入,要按i
inoreabb pra printf("\n");<esc>5h
"文件补全函数的设置的设置
set omnifunc=syntaxcomplete#Complete
autocmd FileType python set omnifunc=pythoncomplete#Complete
autocmd FileType java set omnifunc=javacomplete#Complete
if has("autocmd") && exists("+omnifunc")
     autocmd Filetype *
   \ if &omnifunc == "" |
   \   setlocal omnifunc=syntaxcomplete#Complete |
   \ endif
endif
""""""""""""""缩进、补全设置

""""""""""""""自动命令、键盘映射
nnoremap <silent> <F12> :!ctags.exe -R --c++-kinds=+px --c-kinds=+px --fields=+niazS --extras=+q --output-format=e-ctags .<CR><CR>
" add current directory's generated tags file to available tags
execute "set tags=./.tags;,.tags;" . escape(g:VIM_DATA_PATH, ' ') . "/include/tags"

" Ctrl+Enter也可以切换buffer
map C-Enter C-Tab
nnoremap <silent> <F4> :tabprevious<CR>
nnoremap <silent> <F3> :Grep<CR>
"选中一段文字并全文搜索这段文字
":vnoremap <silent> ,/ y/<C-R>=escape(@", '\\/.*$^~[]')<CR><CR>
":vnoremap <silent> ,? y?<C-R>=escape(@", '\\/.*$^~[]')<CR><CR>
"可视模式下查找关键字,相当于普通模式下的"*"
vmap g/  y/<C-R>=escape(@", '\\/.*$^~[]')<CR><CR>
"取消当前搜索高亮
nmap <silent> <CR> :nohl<CR>
"重新读取vimrc文件
map <silent> <leader>ss :exec "source " . g:VIMRC_PATH<cr>
"编辑vimrc文件
map <silent> <leader>ee :exec "e " g:VIMRC_PATH<cr>
"在当前文件中快速查找光标下的单词
nmap <leader>lv :lv /<c-r>=expand("<cword>")<cr>/ %<cr>:lw<cr>
"编辑状态保存,这里会保存一些inormap的状态,可能会导致修改了但是没生效,需要找出原因
"au BufWinLeave *.[ch] silent mkview
"au BufWinEnter *.[ch] silent loadview
"保存session与viminfo
set sessionoptions=blank,buffers,folds,help,options,tabpages,winsize,unix,slash,sesdir
"au BufWinLeave * silent mksession ~/UnixProgram/vim_session.vim
"au BufWinEnter * silent source ~/UnixProgram/vim_session.vim
""au BufWinLeave * silent wviminfo ~/UnixProgram/vim_viminfo.vim
""au BufWinEnter * silent rviminfo ~/UnixProgram/vim_viminfo.vim
"把当前的单词全部变成大写，已经有快捷键gu和gU了
"inoremap <silent> <C-Y> <esc>gUiwea
"C语言中用来给打出if(condition)和while(condition)等语句后自动补花括号和末尾注释
inoremap <silent> <C-Y><C-Y> {<CR>}<esc>%hv^y<C-O>a//<esc>pO
"C语言中用来给if、while等语句的右花括号后增加或更新注释,必须先定位到if、while那一行并且处于插入模式
inoremap <silent> <C-Y><C-U> <esc>$hv^y$%a<SPACE><esc>d$a//<esc>"0p
"修改本文件第一行的信息
nmap <silent> ti ggddO"modified at <C-R>=strftime("%Y-%m-%d %H:%M:%S")<CR><Esc><C-O>
"nmap <silent> ti :0insert modified at <C-R>=strftime("%Y-%m-%d %H:%M:%S")<CR>
"各个模式下按Ctrl+J可以实现ESC的功能，比较快捷
inoremap <C-J> <ESC>
nnoremap <C-J> <ESC>
cnoremap <C-J> <ESC>
vnoremap <C-J> <ESC>
lnoremap <C-J> <ESC>
xnoremap <C-J> <ESC>
snoremap <C-J> <ESC>
"这里要将visual模式下的 u (变为小写)和 U (变为大写)映射成ESC
"因为经常误按导致变成大小写
vnoremap u <ESC>
vnoremap U <ESC>
"设置visual模式下的 gu 和 gU 为改变大小写
vnoremap gu u
vnoremap gU U
"Ctrl + B向上翻页比较难按
nnoremap <C-Y> <C-B>
"设置粘贴模式
nnoremap <C-K><C-P> <ESC>:setl paste<CR>
"<S-INSERT>:set nopaste<CR>i
nnoremap <C-K><C-N> <ESC>:setl nopaste<CR>
" :A为跳到对应的C或H文件
nnoremap <C-H><C-H> :A<CR>
"cnoremap t Tlist
"查找protobuf的时候message方便
map <silent> <leader>me "zyiw/\<message <C-R>Z\><ENTER>
""""""""""""""自动命令、键盘映射

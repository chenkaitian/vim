""""""""""�������⣬�������롢���������
"set guifontwide="Microsoft YaHei":h12
"����VIM������������
"set lines=50 columns=200
"�������ֵ���󳤶�
set textwidth=250
"colo peachpuff
"source $VIMRUNTIME/vimrc_example.vim
"source $VIMRUNTIME/mswin.vim
"behave mswin
"����VIM��ʹ�õı��뷽ʽ
set encoding=utf-8
"�˵���ʹ�õ����Է���
set langmenu=zh_CN.UTF-8
"�����ļ��ı���
set fileencodings=ucs-bom,utf-8,cp936,gb18030,big5,latin1
"set fencs=utf-8,usc-bom,euc-jp,gb18030,gbk,gb2312,cp936
language message zh_CN.UTF-8
if has("win32")
	set fileencoding=chinese
	"��guiģʽ�£����ô��ڳ�ʼλ��
	"winpos 235 235
	"��GUI�´������
	set winaltkeys=no
	au GUIEnter * simalt ~x
	"��GUIģʽ�¹�겻��˸
	set gcr=a:blinkon0
	"set gcr=a:block-blinkon0
else
	set fileencoding=utf-8
endif
let &termencoding=&encoding
source $VIMRUNTIME/delmenu.vim
source $VIMRUNTIME/menu.vim
""""""""""�������⣬�������롢���������

""""""""""�Զ������
if has("win32")
	"vimrc��λ��
	"ע�������$VIMRUNTIME��ò�Ҫд��˫������
	let g:VIMRC_PATH = $VIMRUNTIME . "/../_vimrc"
	"vim���������ļ���λ��
	let g:VIM_DATA_PATH = $VIM . "/VimData"
else
	let g:VIMRC_PATH = $HOME . "/.vimrc"
	let g:VIM_DATA_PATH = $HOME . "/.vim/VimData"
endif
"���������·��
let g:VIM_PLUGIN_DATA_PATH = g:VIM_DATA_PATH . "/plugins"
""""""""""�Զ������

""""""""""""""һЩϵͳ����
"set guifont=Consolas\ 13
set guifont=Consolas:h11
if &term=="xterm"
	"number of colors
	set t_Co=256
endif
"��ɫ����
"let g:solarized_termcolors=256
"set background=dark
"colorscheme solarized
let g:molokai_original = 1
let g:rehash256 = 1
colorscheme molokai
"����<leader>
let mapleader=","
"����ڴ������±߽�ʱ����߽�6�м���ʼ����
set so=6  "set scrolloff
"��ʾ�к�
set number
"ͻ����ʾ��ǰ��
set cursorline
 "���ò���ϵͳ����ʱ��Ŀ¼
if has("unix")
	"set runtimepath += ~/.vim/textmateOnly
	"set runtimepath+=/home/hunch/.vim/after
endif
"ʹ��˫�ֽڵ�ascii��
set ambiwidth=double
"�Զ����¼����ⲿ�޸�����(����ļ���û�б�vim�޸�)
set autoread
"���õ�ʹ������':first'��':last'�ȵ�ʱ���Զ�д���ļ������ǲ����':exit',':quit'��������Ч"
"set autowrite
"���Զ��л���ǰĿ¼Ϊ��ǰ�ļ����ڵ�Ŀ¼,Ϊ����Ϻ����cscope����tags��cscope�ļ���
set noautochdir
"�����д�����Ϣʱ������beep����
"no bell settings {{{
set noerrorbells
set visualbell
if has('autocmd')
  autocmd guienter * set vb t_vb=
endif
"no bell settings end }}}
"�ڱ༭�����У������½���ʾ���λ�õ�״̬��
"����Ӧ���޸�Ϊ���༭���ڲ���ʾ,�������ڲ���ʾ
set ruler
set rulerformat=%20(%2*%<%f%=\ %m%r\ %3l\ %c\ %p%%%)
"������ʱ����ʾ�Ǹ�Ԯ����ͯ����ʾ
set shortmess=atI
"�Զ�������ʾ
set wrap
"�趨�����е�����Ϊ1
set cmdheight=1
"��ʾ״̬�� (Ĭ��ֵΪ1,�޷���ʾ״̬��)
set laststatus=2
"����״̬����ʾ������
set statusline=%f%m%r%h%w\ [format=%{&ff}]\ [type=%y]\ [ascii=\%03.3b]\ [hex=\%02.2b]\ [pos=%04l,%04v][%p%%]\ [len=%l]
"������һ������ʱ�������ƥ��ģ��������ʾ��һ������
set showmatch
"ѡ��:��һ���ļ�����ʱ,������и�д֮ǰ,�����һ��Դ�ļ��ı���,д����ɺ�ɾ������"
set writebackup
"����nowritebackup,����д����ɺ󱸷ݻᱻ����
set nobackup
"��insertģʽ������ɾ��������ɾ��
set backspace=indent,eol,start
"��ǿģʽ�е��������Զ���ɲ���(���簴tab�����Զ��������)
set wildmenu
"������ʱ�����ִ�Сд
set ignorecase
"��ignorecaseΪonʱ���������һ����д��ĸ�����Զ��ر�ignorecase����������"*","#","gd"����
set smartcase
"�ڽ�����ǰ�������ʱ���������з���ģʽ�Ĺؼ���
set hlsearch
"���������¼����ַ�ʱ���������ϵĹؼ���
set incsearch
"���ڵ����м����
set linebreak
"��ʾ���½���ͨģʽ�µ�����״̬
set showcmd
"�����۵�����,���ó�syntax��������
"set fdm=indent
"�����ƶ�ʱ����ʲô��ĸ�ͷ�������һ������
set iskeyword=_,$,@,%,#,48-57,192-255
"���ð�������
if version >= 603
  set helplang=cn
endif
"���Լ����̵�ʱ���Զ����صĽű�
if getfsize(".vimscript") > 0
  source .vimscript
endif

:runtime! ftplugin/man.vim
"�����ļ������������ͨģʽ�¼���'K'ʱ��������ⲿ������в���
set keywordprg=dicty
"��ʹ�ý����ļ�
set noswapfile

"�˳� vim ����Ȼ����ִ�� undo ����
try
    "set undodir=$VIM/VimData/undo
    "set undofile
catch

endtry
""""""""""""""һЩϵͳ����

""""""""""""""�﷨���ļ�����
"�����C��CPP��VIM�ļ������Զ��Ƴ���β�Ŀո�
au BufWritePre * call RemoveTrailingSpace()
"�Ƴ���β�Ŀո�
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
""""""""""""""�﷨���ļ�����

""""""""""""""��������ȫ����
"���ò�ȫʱʹ�õķ�ʽ(��menuΪ�����˵�)
set completeopt=longest,menu
" �̳�ǰһ�е�������ʽ���ر������ڶ���ע��
set autoindent
"ΪC�����ṩ�Զ�����
"set smartindent
" ʹ��C��ʽ������
set cindent
" �Զ�����ÿһ��ʹ�õĿո���Ŀ
set shiftwidth=4
" tab ������Ŀո���
set tabstop=4
" ���� tab ��ʱʵ��ռ�е�����
set softtabstop=4
" ���� tab ��ʱ��ʹ�ú��������Ŀո����
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
"��ȫ�ļ���ʱ���Ե��ļ�
set suffixes=.bak,~,.o,.info,.swp,.aux,.bbl,.blg,.dvi,.lof,.log,.log,.ps,.toc
"�Զ��滻�Ĳ�ȫ
ab #i #include
ab #d #define
ab #ssh #!/bin/bash<CR>
"����pra�Զ���ȫprintf,���ҽ���궨λ��\n֮ǰw���Ҫ��������,Ҫ��i
inoreabb pra printf("\n");<esc>5h
"�ļ���ȫ���������õ�����
set omnifunc=syntaxcomplete#Complete
autocmd FileType python set omnifunc=pythoncomplete#Complete
autocmd FileType java set omnifunc=javacomplete#Complete
if has("autocmd") && exists("+omnifunc")
     autocmd Filetype *
   \ if &omnifunc == "" |
   \   setlocal omnifunc=syntaxcomplete#Complete |
   \ endif
endif
""""""""""""""��������ȫ����

""""""""""""""�Զ��������ӳ��
nnoremap <silent> <F12> :!ctags.exe -R --c++-kinds=+px --c-kinds=+px --fields=+niazS --extras=+q --output-format=e-ctags .<CR><CR>
" add current directory's generated tags file to available tags
execute "set tags=./.tags;,.tags;" . escape(g:VIM_DATA_PATH, ' ') . "/include/tags"

" Ctrl+EnterҲ�����л�buffer
map C-Enter C-Tab
nnoremap <silent> <F4> :tabprevious<CR>
nnoremap <silent> <F3> :Grep<CR>
"ѡ��һ�����ֲ�ȫ�������������
":vnoremap <silent> ,/ y/<C-R>=escape(@", '\\/.*$^~[]')<CR><CR>
":vnoremap <silent> ,? y?<C-R>=escape(@", '\\/.*$^~[]')<CR><CR>
"����ģʽ�²��ҹؼ���,�൱����ͨģʽ�µ�"*"
vmap g/  y/<C-R>=escape(@", '\\/.*$^~[]')<CR><CR>
"ȡ����ǰ��������
nmap <silent> <CR> :nohl<CR>
"���¶�ȡvimrc�ļ�
map <silent> <leader>ss :exec "source " . g:VIMRC_PATH<cr>
"�༭vimrc�ļ�
map <silent> <leader>ee :exec "e " g:VIMRC_PATH<cr>
"�ڵ�ǰ�ļ��п��ٲ��ҹ���µĵ���
nmap <leader>lv :lv /<c-r>=expand("<cword>")<cr>/ %<cr>:lw<cr>
"�༭״̬����,����ᱣ��һЩinormap��״̬,���ܻᵼ���޸��˵���û��Ч,��Ҫ�ҳ�ԭ��
"au BufWinLeave *.[ch] silent mkview
"au BufWinEnter *.[ch] silent loadview
"����session��viminfo
set sessionoptions=blank,buffers,folds,help,options,tabpages,winsize,unix,slash,sesdir
"au BufWinLeave * silent mksession ~/UnixProgram/vim_session.vim
"au BufWinEnter * silent source ~/UnixProgram/vim_session.vim
""au BufWinLeave * silent wviminfo ~/UnixProgram/vim_viminfo.vim
""au BufWinEnter * silent rviminfo ~/UnixProgram/vim_viminfo.vim
"�ѵ�ǰ�ĵ���ȫ����ɴ�д���Ѿ��п�ݼ�gu��gU��
"inoremap <silent> <C-Y> <esc>gUiwea
"C���������������if(condition)��while(condition)�������Զ��������ź�ĩβע��
inoremap <silent> <C-Y><C-Y> {<CR>}<esc>%hv^y<C-O>a//<esc>pO
"C������������if��while�������һ����ź����ӻ����ע��,�����ȶ�λ��if��while��һ�в��Ҵ��ڲ���ģʽ
inoremap <silent> <C-Y><C-U> <esc>$hv^y$%a<SPACE><esc>d$a//<esc>"0p
"�޸ı��ļ���һ�е���Ϣ
nmap <silent> ti ggddO"modified at <C-R>=strftime("%Y-%m-%d %H:%M:%S")<CR><Esc><C-O>
"nmap <silent> ti :0insert modified at <C-R>=strftime("%Y-%m-%d %H:%M:%S")<CR>
"����ģʽ�°�Ctrl+J����ʵ��ESC�Ĺ��ܣ��ȽϿ��
inoremap <C-J> <ESC>
nnoremap <C-J> <ESC>
cnoremap <C-J> <ESC>
vnoremap <C-J> <ESC>
lnoremap <C-J> <ESC>
xnoremap <C-J> <ESC>
snoremap <C-J> <ESC>
"����Ҫ��visualģʽ�µ� u (��ΪСд)�� U (��Ϊ��д)ӳ���ESC
"��Ϊ�����󰴵��±�ɴ�Сд
vnoremap u <ESC>
vnoremap U <ESC>
"����visualģʽ�µ� gu �� gU Ϊ�ı��Сд
vnoremap gu u
vnoremap gU U
"Ctrl + B���Ϸ�ҳ�Ƚ��Ѱ�
nnoremap <C-Y> <C-B>
"����ճ��ģʽ
nnoremap <C-K><C-P> <ESC>:setl paste<CR>
"<S-INSERT>:set nopaste<CR>i
nnoremap <C-K><C-N> <ESC>:setl nopaste<CR>
" :AΪ������Ӧ��C��H�ļ�
nnoremap <C-H><C-H> :A<CR>
"cnoremap t Tlist
"����protobuf��ʱ��message����
map <silent> <leader>me "zyiw/\<message <C-R>Z\><ENTER>
""""""""""""""�Զ��������ӳ��

"YCM ��ȫ�˵���ɫ
highlight Pmenu ctermfg=2 ctermbg=3 guifg=#005f87 guibg=#EEE8D5
" ѡ����
highlight PmenuSel ctermfg=2 ctermbg=3 guifg=#AFD700 guibg=#106900
"��ȫ������ע����ͬ����Ч
let g:ycm_complete_in_comments=1
"���� vim ���� .ycm_extra_conf.py �ļ���������ʾ
let g:ycm_confirm_extra_conf=0
"���� YCM ��ǩ��ȫ����
"let g:ycm_collect_identifiers_from_tags_files=1
"���� C++ ��׼��tags
"set tags+=/data/misc/software/misc./vim/stdcpp.tags
"YCM ���� OmniCppComplete ��ȫ���棬�������ݼ�
"inoremap <leader>; <C-x><C-o>
"��ȫ���ݲ��Էָ��Ӵ�����ʽ���֣�ֻ��ʾ��ȫ�б�
set completeopt-=preview
"�ӵ�һ�������ַ��Ϳ�ʼ����ƥ����
let g:ycm_min_num_of_chars_for_completion=1
"��ֹ����ƥ���ÿ�ζ���������ƥ����
let g:ycm_cache_omnifunc=0
"�﷨�ؼ��ֲ�ȫ
let g:ycm_seed_identifiers_with_syntax=1
"�޸Ķ�C�����Ĳ�ȫ��ݼ���Ĭ����CTRL + space���޸�ΪALT + ;
let g:ycm_key_invoke_completion = '<M-;>'

let g:ycm_global_ycm_extra_conf='C:\\Program Files\\vim\\vim.ycm_extra_conf.py'
"change the compiler to g++ to support c++11.
let g:syntastic_cpp_compiler = 'g++'
"set the options of g++ to suport c++11.
let g:syntastic_cpp_compiler_options = '-std=c++11 -stdlib=libc++'
"let g:ycm_path_to_python_interpreter='D:\\Program Files\\Python36\\python.exe'
"let g:ycm_server_python_interpreter='D:\\Program Files\\Python36\\python.exe'
"let g:ycm_global_ycm_extra_conf='D:\\Program Files (x86)\\GVIM\\vim.ycm_extra_conf.py'
"let g:ycm_confirm_extra_conf=1
"����ת�����崦�Ŀ�ݼ�
nnoremap <leader>gl :YcmCompleter GoToDeclaration <C-R>=expand("<cword>")<CR><CR>
nnoremap <leader>gf :YcmCompleter GoToDefinition <C-R>=expand("<cword>")<CR><CR>
nnoremap <leader>gg :YcmCompleter GoToDefinitionElseDeclaration <C-R>=expand("<cword>")<CR><CR>
let g:ycm_key_list_select_completion = ['<c-n>', '<Down>']
let g:ycm_key_list_previous_completion = ['<c-p>', '<Up>']
let g:ycm_key_invoke_completion = ''
"more than n kb to disable ycm
let g:ycm_disable_for_files_larger_than_kb = 2000
"��ʼ��ȫ���ַ���
let g:ycm_min_num_of_chars_for_completion = 2
"�뿪����ģʽ���Զ��ر�Ԥ������
autocmd InsertLeave * if pumvisible() == 0|pclose|endif
let g:ycm_use_ultisnips_completer = 1
" ����syntastic����python���
let g:syntastic_ignore_files=[".*\.py$"]
"�﷨���,��syntastic�г�ͻ
let g:ycm_show_diagnostics_ui = 0
let g:ycm_enable_diagnostic_signs = 0
let g:ycm_enable_diagnostic_highlighting = 0
let g:ycm_echo_current_diagnostic = 0
let g:ycm_error_symbol = '>>'
let g:ycm_warning_symbol = '>*'
"�����ر�YCM
"let g:ycm_auto_trigger=1

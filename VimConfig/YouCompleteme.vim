"YCM 补全菜单配色
highlight Pmenu ctermfg=2 ctermbg=3 guifg=#005f87 guibg=#EEE8D5
" 选中项
highlight PmenuSel ctermfg=2 ctermbg=3 guifg=#AFD700 guibg=#106900
"补全功能在注释中同样有效
let g:ycm_complete_in_comments=1
"允许 vim 加载 .ycm_extra_conf.py 文件，不再提示
let g:ycm_confirm_extra_conf=0
"开启 YCM 标签补全引擎
"let g:ycm_collect_identifiers_from_tags_files=1
"引入 C++ 标准库tags
"set tags+=/data/misc/software/misc./vim/stdcpp.tags
"YCM 集成 OmniCppComplete 补全引擎，设置其快捷键
"inoremap <leader>; <C-x><C-o>
"补全内容不以分割子窗口形式出现，只显示补全列表
set completeopt-=preview
"从第一个键入字符就开始罗列匹配项
let g:ycm_min_num_of_chars_for_completion=1
"禁止缓存匹配项，每次都重新生成匹配项
let g:ycm_cache_omnifunc=0
"语法关键字补全
let g:ycm_seed_identifiers_with_syntax=1
"修改对C函数的补全快捷键，默认是CTRL + space，修改为ALT + ;
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
"设置转到定义处的快捷键
nnoremap <leader>gl :YcmCompleter GoToDeclaration <C-R>=expand("<cword>")<CR><CR>
nnoremap <leader>gf :YcmCompleter GoToDefinition <C-R>=expand("<cword>")<CR><CR>
nnoremap <leader>gg :YcmCompleter GoToDefinitionElseDeclaration <C-R>=expand("<cword>")<CR><CR>
let g:ycm_key_list_select_completion = ['<c-n>', '<Down>']
let g:ycm_key_list_previous_completion = ['<c-p>', '<Up>']
let g:ycm_key_invoke_completion = ''
"more than n kb to disable ycm
let g:ycm_disable_for_files_larger_than_kb = 2000
"开始补全的字符数
let g:ycm_min_num_of_chars_for_completion = 2
"离开插入模式后自动关闭预览窗口
autocmd InsertLeave * if pumvisible() == 0|pclose|endif
let g:ycm_use_ultisnips_completer = 1
" 禁用syntastic来对python检查
let g:syntastic_ignore_files=[".*\.py$"]
"语法检测,与syntastic有冲突
let g:ycm_show_diagnostics_ui = 0
let g:ycm_enable_diagnostic_signs = 0
let g:ycm_enable_diagnostic_highlighting = 0
let g:ycm_echo_current_diagnostic = 0
let g:ycm_error_symbol = '>>'
let g:ycm_warning_symbol = '>*'
"开启关闭YCM
"let g:ycm_auto_trigger=1

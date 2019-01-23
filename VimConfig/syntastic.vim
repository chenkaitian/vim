""""""""""(syntastic������
let g:syntastic_enable_signs = 1
let g:syntastic_error_symbol='X'
let g:syntastic_warning_symbol='?'
let g:syntastic_always_populate_loc_list = 1
let g:syntastic_check_on_open = 1
let g:syntastic_enable_balloons = 1
let g:syntastic_auto_loc_list = 0
let g:syntastic_python_checkers = ['pyflakes']
let g:syntastic_auto_loc_list = 1
let g:syntastic_loc_list_height = 5
let g:syntastic_check_on_wq = 0
let g:syntastic_enable_highlighting=1
let g:syntastic_cpp_compiler = 'clang++'
let g:syntastic_cpp_compiler_options = ' -std=c++11 -stdlib=libc++'
"�޸�syntasticʹ��:lnext��:lprev���ֵ���ת���⣬ͬʱ�޸ļ���ӳ��ʹ��sn��sp������ת
function! <SID>LocationPrevious()
  try
    lprev
  catch /^Vim\%((\a\+)\)\=:E553/
    llast
  endtry
endfunction
function! <SID>LocationNext()
  try
    lnext
  catch /^Vim\%((\a\+)\)\=:E553/
    lfirst
  endtry
endfunction
nnoremap <silent> <Plug>LocationPrevious :<C-u>exe 'call <SID>LocationPrevious()'<CR>
nnoremap <silent> <Plug>LocationNext :<C-u>exe 'call <SID>LocationNext()'<CR>
nmap <silent> sp <Plug>LocationPrevious
nmap <silent> sn <Plug>LocationNext
"�ر�syntastic�﷨���, ��긴�ƴ���ʱ�õ�, ��ֹ�Ѵ����־��������
nnoremap <silent> <Leader>ec :SyntasticToggleMode<CR>
function! ToggleErrors()
    let old_last_winnr = winnr('$')
    lclose
    if old_last_winnr == winnr('$')
        " Nothing was closed, open syntastic error location panel
        Errors
    endif
endfunction
""""""""""syntastic������)

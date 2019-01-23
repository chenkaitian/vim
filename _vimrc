set nocompatible
filetype off

" �� pathogen ����Ҳ���ڶ���Ŀ¼�У���ָ����·��
"runtime bundle/pathogen/autoload/pathogen.vim
" ���� pathogen
"execute pathogen#infect()

set rtp+=$VIM/vimfiles/bundle/Vundle.vim/
set rtp+=$VIM

call vundle#begin('$VIM/vimfiles/bundle/')
Plugin 'VundleVim/Vundle.vim'
Plugin 'vim-scripts/a.vim'
Plugin 'vim-scripts/mru.vim'
Plugin 'majutsushi/tagbar'
Plugin 'scrooloose/nerdtree'
Plugin 'bufexplorer.zip'
Plugin 'taglist.vim'
Plugin 'ShowMarks'
Plugin 'Valloric/YouCompleteMe'
Plugin 'tomasr/molokai'
Plugin 'altercation/vim-colors-solarized'
Plugin 'jiangmiao/auto-pairs'
Plugin 'SirVer/ultisnips'
Plugin 'honza/vim-snippets'
Plugin 'ludovicchabant/vim-gutentags'
"Plugin 'Yggdroot/indentLine'
"Plugin 'w0rp/ale'
"Plugin 'scrooloose/syntastic'

"Plugin 'Shougo/neocomplcache'
"Plugin 'Shougo/neocomplete.vim'
"Plugin 'Shougo/neosnippet.vim'
"Plugin 'gmarik/Vundle.vim'
"Plugin 'kien/ctrlp.vim'
"Plugin 'bling/vim-airline'
"Plugin 'vim-airline/vim-airline-themes'
"Plugin 'majutsushi/tagbar'
"Plugin 'jlanzarotta/bufexplorer'
"Plugin 'godlygeek/csapprox'
"Plugin 'justinmk/vim-syntax-extra'
"Plugin 'vim-utils/vim-man'
"Plugin 'easymotion/vim-easymotion'
"Plugin 'terryma/vim-multiple-cursors'
"Plugin 'terryma/vim-expand-region'
"Plugin 'vim-scripts/peaksea'
"Plugin 'tpope/vim-commentary'
"Plugin 'ifdef-highlighting'
call vundle#end()

"�﷨��������ʾ
syntax enable
syntax on
"����ļ������� ����codesnip
filetype on
filetype plugin on
filetype indent on
"filetype plugin indent on

source $VIM/VimConfig/basic.vim
source $VIM/VimConfig/plugins.vim

" ��ֹ��ʾ������
"set guioptions-=l
"set guioptions-=L
"set guioptions-=r
"set guioptions-=R
" ��ֹ��ʾ�˵��͹�����
" set guioptions-=m
" set guioptions-=T

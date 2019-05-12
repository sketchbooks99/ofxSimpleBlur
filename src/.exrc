if &cp | set nocp | endif
map Q gq
let s:cpo_save=&cpo
set cpo&vim
vmap gx <Plug>NetrwBrowseXVis
nmap gx <Plug>NetrwBrowseX
vnoremap <silent> <Plug>NetrwBrowseXVis :call netrw#BrowseXVis()
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#BrowseX(expand((exists("g:netrw_gx")? g:netrw_gx : '<cfile>')),netrw#CheckIfRemote())
inoremap  u
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set background=dark
set backspace=indent,eol,start
set clipboard=unnamed
set display=truncate
set fileencodings=ucs-bom,utf-8,default,latin1
set helplang=ja
set history=200
set incsearch
set langnoremap
set nolangremap
set laststatus=2
set nrformats=bin,hex
set ruler
set scrolloff=5
set showcmd
set showmatch
set statusline=%f%h%m%r\ [%{&ff}]\ (%{strftime(\"%H:%M\ %d/%m/%Y\",getftime(expand(\"%:p\")))})%=%l,%c%V\ %P
set ttimeout
set ttimeoutlen=100
set visualbell
set wildmenu
set wildmode=list:longest,longest:full
set window=57
" vim: set ft=vim :

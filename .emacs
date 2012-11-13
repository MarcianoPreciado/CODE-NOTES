(custom-set-variables	  ;
  ;; custom-set-variables was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(column-number-mode t)
 '(cua-mode t nil (cua-base))
 '(default-frame-alist (quote ((menu-bar-lines . 1) (tool-bar-lines . 1))))
 '(show-paren-mode t) 
 '(text-mode-hook (quote (turn-on-auto-fill text-mode-hook-identify)))
 '(setq default-fill-column 80);默认显示 80列就换行
)
  ; turn on paren match highlighting
(custom-set-faces
  ;; custom-set-faces was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 )

;; 打开图片显示功能
(auto-image-file-mode t)

;;package 24
(when
    (load
     (expand-file-name "~/.emacs.d/elpa/package.el"))
  (package-initialize))

(require 'package)
(add-to-list 'package-archives '("melpa" . "http://melpa.milkbox.net/packages/") t)

;; load ErgoEmacs keybinding
(load "~/.emacs.d/elpa/ergoemacs-keybindings-20120608/ergoemacs-mode")
;; turn on minor mode ergoemacs-mode
(ergoemacs-mode 1)

(require 'undo-tree)
(global-undo-tree-mode 1)
(defalias 'redo 'undo-tree-redo)
(global-set-key (kbd "C-z") 'undo) ; 【Ctrl+z】
(global-set-key (kbd "C-S-z") 'redo) ; 【Ctrl+Shift+z】

(defadvice kill-ring-save (before slick-copy activate compile)
  "When called interactively with no active region, copy the current line."
  (interactive
   (if mark-active
       (list (region-beginning) (region-end))
     (progn
       (message "Current line is copied.")
       (list (line-beginning-position) (line-beginning-position 2)) ) ) ))

(defadvice kill-region (before slick-copy activate compile)
  "When called interactively with no active region, cut the current line."
  (interactive
   (if mark-active
       (list (region-beginning) (region-end))
     (progn
       (list (line-beginning-position) (line-beginning-position 2)) ) ) ))
(global-set-key (kbd "<f2>") 'kill-region) ; cut.
(global-set-key (kbd "<f3>") 'kill-ring-save) ; copy.
(global-set-key (kbd "<f4>") 'yank) ; paste.

(transient-mark-mode 1) ; highlight text selection
(delete-selection-mode 1) ; delete seleted text when typing
(global-hl-line-mode 1)
(global-linum-mode 1) ; display line numbers in margin. Emacs 23 only.
(column-number-mode 1)

(recentf-mode 1) ; keep a list of recently opened files
(global-visual-line-mode 1) ; 1 for on, 0 for off.
;(setq pop-up-frames t) ; each file opens in new window
(auto-fill-mode 1)

(defun my-backup-file-name (fpath)
  "Return a new file path of a given file path.
If the new path's directories does not exist, create them."
  (let (backup-root bpath)
    (setq backup-root "~/.emacs.d/emacs-backup")
    (setq bpath (concat backup-root fpath "~"))
    (make-directory (file-name-directory bpath) bpath)
    bpath
  )
)
(setq make-backup-file-name-function 'my-backup-file-name)

;; allow dired to be able to delete or copy a whole dir.
(setq dired-recursive-copies (quote always))
(setq dired-recursive-deletes (quote top))

(setq dired-dwim-target t) ;copy dir2dir use split window
(require 'dired+)
(require 'bookmark+)
(setq bookmark-default-file  (concat user-emacs-directory "bookmarks"))

(require 'yasnippet)
(yas/initialize)

(require 'color-theme)
(color-theme-sanityinc-tomorrow-night)

(defalias 'list-buffers 'ibuffer)
(defalias 'yes-or-no-p 'y-or-n-p)   ; y or n is enough
(defalias 'perl-mode 'cperl-mode);
(defalias 'man 'woman)
(prefer-coding-system 'utf-8)

;; (setq file-name-coding-system 'gbk)
;; (set-terminal-coding-system 'gbk)
;; (set-keyboard-coding-system 'gbk)
;; (setq locale-coding-system 'gbk)
;; (set-selection-coding-system 'gbk)
;; (set-clipboard-coding-system 'ctext)
;; (set-clipboard-coding-system 'gbk)
;; (set-terminal-coding-system 'gbk)
;; (set-buffer-file-coding-system 'gbk)
;; (modify-coding-system-alist 'process "." 'gbk)
;; (setq default-process-coding-system '(gbk . gbk))
(add-to-list 'load-path "~/.emacs.d/elpa/auto-complete-20120327")
    (require 'auto-complete-config)
    (add-to-list 'ac-dictionary-directories "~/.emacs.d/elpa/auto-complete-20120327") 
    
    (require 'auto-complete-clang)
    
    (setq ac-auto-start nil)
    (setq ac-quick-help-delay 0.5)
    (ac-set-trigger-key "TAB")
    (define-key ac-mode-map  [(control tab)] 'auto-complete)
    (define-key ac-mode-map  [(control tab)] 'auto-complete)
    (defun my-ac-config ()
      (setq-default ac-sources '(ac-source-abbrev ac-source-dictionary ac-source-words-in-same-mode-buffers))
      (add-hook 'emacs-lisp-mode-hook 'ac-emacs-lisp-mode-setup)
      ;; (add-hook 'c-mode-common-hook 'ac-cc-mode-setup)
      (add-hook 'ruby-mode-hook 'ac-ruby-mode-setup)
      (add-hook 'css-mode-hook 'ac-css-mode-setup)
      (add-hook 'auto-complete-mode-hook 'ac-common-setup)
      (global-auto-complete-mode t))
    (defun my-ac-cc-mode-setup ()
      (setq ac-sources (append '(ac-source-clang ac-source-yasnippet) ac-sources)))
    (add-hook 'c-mode-common-hook 'my-ac-cc-mode-setup)
    ;; ac-source-gtags
    (my-ac-config)
(setq ac-clang-flags
      (mapcar (lambda (item)(concat "-I" item))
              (split-string
               "
 /usr/include/c++/4.6
 /usr/include/c++/4.6/x86_64-linux-gnu/.
 /usr/include/c++/4.6/backward
 /usr/lib/gcc/x86_64-linux-gnu/4.6.1/include
 /usr/include/qt4
 /usr/local/include
 /usr/lib/gcc/x86_64-linux-gnu/4.6.1/include-fixed
 /usr/include/x86_64-linux-gnu
 /usr/include
"
               )))

;;;Tab 转空格  
(setq-default indent-tabs-mode  nil)
  
(require 'google-c-style)
(add-hook 'c-mode-common-hook 'google-set-c-style)
(add-hook 'c-mode-common-hook 'google-make-newline-indent)
  
;;设置自动模式  
(add-to-list 'magic-mode-alist'("#include" . c++-mode))  
(add-to-list 'magic-mode-alist'("#ifndef" . c++-mode))  

(require 'cedet)
(global-ede-mode t)

(setq semanticdb-default-save-directory
(expand-file-name "~/.emacs.d/semanticdb"))

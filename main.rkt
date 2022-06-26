;oktay ozel
;2019400108
;compiling: yes
;complete: yes
#lang racket

(provide (all-defined-out))

; 10 points
(define := (lambda (var value) (list var value ) ) )

; 10 points
(define -- (lambda args (list 'let args)))

; 10 points
(define @ (lambda (bindings expr) (append bindings expr)))

; 20 points
;(define split_at_delim (lambda (delim args) 0))


(define split_at_delim ( lambda(delim args) (split-helper delim args  '() '() ) ) )

;(define split-helper (lambda(delim args ans)  ans))

(define split-helper (lambda(delim args ans curans)  (cond
                                                      ((null? args) (my-reverse(cons (my-reverse curans) ans)))
                                                      ((eqv? (car args) delim ) (split-helper delim (cdr args) (cons (my-reverse curans) ans) '() ) )
                                                      (else (split-helper delim (cdr args) ans  (cons  (car args) curans ) ) )
                                              )
                     )
)

(define (my-reverse ls)
  (define (my-reverse-2 ls acc)
    (if (null? ls)
      acc
      (my-reverse-2 (cdr ls) (cons (car ls) acc))))
  (my-reverse-2 ls '()))



(define split_at_delim ( lambda (delim args) (split-helper delim args '() ) ) )

(define split-helper (lambda (delim args return) (cond
                                                 (null? args ans )
                                                 ((eqv? (car args) delim) (split-helper delim (cdr args) (cons) ) )
                                                 (else ())
                     )
)

;( parse_expr ( car (split_at_first '+  '(1 + 3)) ) )
; (parse_expr '(1 + 3))
; 30 points
;(define parse_expr (lambda (expr) 0))


(define parse_binding_list (lambda (bl) (cond 
                                         ( (and (list? bl) (= (length bl) 1 ) )  ( parse_binding_list (car bl ) )  )
                                         (else (map parse_assignment ( reverse ( cdr (reverse (split_at_delim '-- bl) ) ) ) ))
                                         )
                             )
)
(define parse_assignment (lambda (ass) (cond
                                            ( ( number? ass ) ass)
                                            ( ( member? ass '(a b c d e f g h i j k l m n o p q r s t u y v z w x ) ) ass)
                                            ( (member ':= ass ) (eval (list ':= ( caar (split_at_delim ':= ass ) )  (caadr (split_at_delim ':= ass) )  ) ) )
                                            ; parantezden kurtarma eklenebilirp
                                          )
                         )
)

;(parse_assignment '('x := 5))


(define parse_expr (lambda (expr) (cond
                                  
                                    ( ( number? expr ) expr)
                                    
                                    ( ( member? expr '(a b c d e f g h i j k l m n o p q r s t u y v z w x ) ) expr)                                    
                                 
                                 
                                    ( (member? '+ expr) (append '(+)  (foldr cons '() (map parse_expr (split_at_delim '+ expr) ) ) ) )
                                   
                                    ( (member? '* expr) (append '(*)  (foldr cons '() (map parse_expr (split_at_delim '* expr) ) ) ) )
                                    
                                    ( (and (list? expr) (= (length expr) 1 ) )  ( parse_expr (car expr ) )  )
                                    
                                    ( (member? '@ expr) (list 'let ( parse_binding_list ( car (split_at_delim '@ expr ) ) ) (parse_expr (cdr (split_at_delim '@ expr) ) )     ))
                                    
                                  )
                   )
)


(define split_at_first ( lambda(delim args) (split-helper-first delim args  '() ) ) )


(define split-helper-first (lambda(delim args ans )  (cond
                                                      ((null? args) ans)
                                                      (( eqv? (length args) 1) (cons ans (cdr args) ) )
                                                      ((eqv? (car args) delim ) (list ans (cdr args)) ) 
                                                      (else (split-helper-first delim (cdr args) (cons (car args) ans ) ) )
                                                      )
                     )
)

(define (member? u lst)
  (not (equal? (member u lst) #f)))


(define howmany ( lambda (listt ans) (howmanyhelper listt 0) ) )

(define howmanyhelper ( lambda (listt ans) (cond 
                                    ((null? listt) ans)
                                    ((eqv? (car listt) '--) (howmanyhelper (cdr listt) (+ 1 ans)))
                                    (else (howmanyhelper (cdr listt) ans ))
                                   )
                 )
)












; 20 points
;(define eval_expr (lambda (expr) 0))

(define eval_expr (lambda (expr)
                          (eval (parse_expr expr))
                  )
)
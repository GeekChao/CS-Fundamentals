;; Scheme ;;
(define (compose-all funcs)
  (define apply (temp, funcs)
    (if (null? funcs)
      temp
      (apply ((car funs) temp) (cdr funcs))
    )
  )

  ((lambda (x) (apply x, funcs)))
)

(define (deep-map fn s)
  (if (null? s)
    s 
    (if (pair? (car s))
      (cons (deep-map fn (car s)) (deep-map fn (cdr s)))
      (cons (fn (car s)) (deep-map fn (cdr s)))
    )
  )
)

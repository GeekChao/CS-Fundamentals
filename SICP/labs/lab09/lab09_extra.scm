;; Extra Scheme Questions ;;


; Q5
(define lst
  (list (list 1) 2 (cons 3 4) 5)
)

; Q6
(define (composed f g)
  (define (h (x))
    (f (g x))
  )
)

; Q7
(define (remove item lst)
  (if (null? lst) 
    lst
    (if (= (car lst) item)
      (remove item (cdr lst))
      (cons (car lst) (remove item (cdr lst)))
    )
  )
)


;;; Tests
(remove 3 nil)
; expect ()
(remove 3 '(1 3 5))
; expect (1 5)
(remove 5 '(5 3 5 5 1 4 5 4))
; expect (3 1 4 4)

; Q8
(define (max a b) (if (> a b) a b))
(define (min a b) (if (> a b) b a))
(define (gcd a b)
  (if (zero? (modulo (max a b) (min a b)))
      (min a b)
      (gcd (min a b) (modulo (max a b) (min a b)))
  )
)

;;; Tests
(gcd 24 60)
; expect 12
(gcd 1071 462)
; expect 21

; Q9
(define (no-repeats s)
 (if (null? s)
    s
    (
      cons (car s)
      (no-repeats 
        (filter (lambda (x) (not (= x (car s)))) (cdr s))
      )
    )
 )
)

(no-repeats (list 5 4 5 4 2 2)) 

; Q10
(define (substitute s old new)
  (if (null? s)
    s 
    (if (pair? (car s))
      (cons (substitute (car s) old new) (substitute (cdr s) old new))
      (if (eq? (car s) old)
        (cons new (substitute (cdr s) old new))
        (cons (car s) (substitute (cdr s) old new))
      )
    )
  )
)

(substitute (list 5 4 5 4 2 2) 5 11) 
(substitute (list 5 (list 1 5 (list 3 5 12)) 5 4 2 2) 5 11) 

; Q11
(define (sub-all s olds news)
  (if (or (null? olds) (null? s))
    s 
    (sub-all (substitute s (car olds) (car news)) (cdr olds) (cdr news))
  )
)
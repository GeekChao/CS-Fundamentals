;; Scheme ;;

; Q2
(define (over-or-under x y)
  (cond 
      ((> x y) 1)
      ((< x y) -1)
      (else 0)
  )
)

;;; Tests
(over-or-under 1 2)
; expect -1
(over-or-under 2 1)
; expect 1
(over-or-under 1 1)
; expect 0

; Q3
(define (filter f lst)
  (define (iterate lst)
      (if (null? lst) 
        lst
        (if (not (f (car lst))) 
          (iterate (cdr lst))
          (cons (car lst) (iterate (cdr lst)))
        ) 
      )
  )

  (iterate lst)
)

;;; Tests
(define (even? x)
  (= (modulo x 2) 0))
(filter even? '(0 1 1 2 3 5 8 9))
; expect (0 2 8)

; Q4
(define (make-adder num)
  (lambda (x) (+ x num))
)

;;; Tests
(define adder (make-adder 5))
(adder 8)
; expect 13
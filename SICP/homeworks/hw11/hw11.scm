(define (find s predicate)
  'YOUR-CODE-HERE
  (if (null? s)
    #f
    (if (predicate (car s))
      (car s)
      (find (cdr-stream s) predicate)
    )
  )
)

(define (scale-stream s k)
  'YOUR-CODE-HERE
  (if (null? s)
    nil
    (cons-stream (* (car s) k) (scale-stream (cdr-stream s) k))
  )
)

(define (has-cycle s)
  'YOUR-CODE-HERE
  (define (track newS oldS)
    (if (eq? (reverse newS) s)
      #t
      (track (cons-stream (car oldS) newS) (cdr-stream oldS))
    )
  )

  (track s s)
)

(define (has-cycle-constant s)
  (let ((slow s)
        (fast (cdr-stream s)))
       (cycle-stepper slow fast)
  )
)
(define (cycle-stepper slow fast)
  (cond ((or (null? fast) (null? (cdr-stream fast))) #f)
        ((eq? fast slow) #t)
        (else (cycle-stepper (cdr-stream slow) (cdr-stream (cdr-stream fast))))
  )
)

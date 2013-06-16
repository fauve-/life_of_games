(ns quil_gol.core
  (:use quil.core))

(def grid (atom {}))
(def game-height (atom 0))
(def game-width (atom 0))


(defn make-grid-element [x y occupado]
  (hash-map (vector x y) occupado))

(defn make-grid [width height]
  (dorun
   (for [w (range width)
         h (range height)
         tmp (make-grid-element w h (if (= (rand-int 20) 0) true false))]
     (swap! grid (fn[g](merge g tmp))))))

;;lol, algorithmic complexity
(defn get-status-from-coord [coords]
  (@grid coords))

(defn get-neighbor-status [coords]
  (for [xaxis [-1 0 1]  yaxis [-1 0 1]
        :when (not= [0 0] [xaxis yaxis])
        :let [x (first coords)
              y (second coords)]]           
    (get-status-from-coord (vector (+ xaxis x) (+ yaxis y)))))
  
(defn sum-occupied-neighbors [grid-element]
  (count (filter identity 
                 (get-neighbor-status (first grid-element)))))

(defn rules[i]
  (let [score (sum-occupied-neighbors i)]
    (if (or (= score 3) (and (= score 2) (second i)))
      :live
      :die)))

(defn update-pass []
  (dorun
   (for [ele @grid](case (rules ele)
                     :live (swap! grid (fn[g](update-in
                                             g
                                             (vector (first ele))
                                             (fn[x y] true) (first ele))))
                     :die  (swap! grid (fn[x]
                                         (update-in
                                          x
                                          (vector (first ele))
                                          (fn[x y] false)(first ele))))))))

(defn setup[]
  (smooth)
  (frame-rate 1)
  (background 255 255 255))

(defn drawgrid[]
  (clojure.pprint/pprint
   (for [y (range @game-width)]
     (apply str
            (for[x (range @game-height)]
              (if (@grid (vector x y))
                \#
                \.))))))



(defn doit []
  (fill 255 255 255)
  (rect 0 0 (* 10 @game-height) (* 10 @game-width))
  (fill 100)
  (doseq [y (range @game-width)]
    (doseq [x (range @game-height)]
      (if  (@grid (vector x y))
        (rect (* 10 x)  (* y 10) 10 10))))
  (update-pass))


(defn main [height width number-of-cells]
  (swap! game-height (fn[x] height))
  (swap! game-width (fn[x] width))
  (if (= (count @grid) 0)
    (make-grid @game-height @game-width)
    (do (swap! grid {}) (make-grid @game-height @game-width)))
  (defsketch alife
    :title "alife"
    :setup setup
    :draw doit
    :size [(* 10 height) (* 10 width)]))

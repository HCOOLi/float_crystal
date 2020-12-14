package main

import (
    "net/http"
	"log"
	"fmt"  
)

func myHandler(w http.ResponseWriter, r *http.Request) {
    fmt.Fprintf(w, "Hello there!\n")
}

func main(){
    http.HandleFunc("/", myHandler)		//	设置访问路由
	log.Fatal(http.ListenAndServe(":18888", http.FileServer(http.Dir("/home/zy/float_crystal"))))
}

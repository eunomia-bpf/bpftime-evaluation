package main

import (
	"log"
	"net/http"
)

func handler(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte("Hello, world!"))
}

func main() {
	http.HandleFunc("/", handler)

	err := http.ListenAndServe(":447", nil)
	if err != nil {
		log.Fatalf("Failed to start server: %v", err)
	}
}

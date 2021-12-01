APP_NAME=grand-prix

test: 
	@echo Test 1
	go run ${APP_NAME}.go || true

clean: 
	rm -rf ${APP_NAME}
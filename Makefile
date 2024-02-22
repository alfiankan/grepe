build-all-cross-platform:
	rm -rf zig-cache && zig build -Dtarget=aarch64-linux-musl --summary all && cp zig-out/bin/grepe bin/grepe-linux-arm64 && \
	rm -rf zig-cache && zig build -Dtarget=x86_64-linux-musl --summary all && cp zig-out/bin/grepe bin/grepe-linux-amd64 && \
	rm -rf zig-cache && zig build -Dtarget=aarch64-macos-none --summary all && cp zig-out/bin/grepe bin/grepe-darwin-arm64 && \
	rm -rf zig-cache && zig build -Dtarget=x86_64-macos-none --summary all && cp zig-out/bin/grepe bin/grepe-darwin-amd64 && \
	rm -rf zig-cache && zig build -Dtarget=aarch64-linux-musl --summary all && cp zig-out/bin/grepe-test bin/grepe-test-linux-arm64 && \
	rm -rf zig-cache && zig build -Dtarget=x86_64-linux-musl --summary all && cp zig-out/bin/grepe-test bin/grepe-test-linux-amd64 && \
	rm -rf zig-cache && zig build -Dtarget=aarch64-macos-none --summary all && cp zig-out/bin/grepe-test bin/grepe-test-darwin-arm64 && \
	rm -rf zig-cache && zig build -Dtarget=x86_64-macos-none --summary all && cp zig-out/bin/grepe-test bin/grepe-test-darwin-amd64



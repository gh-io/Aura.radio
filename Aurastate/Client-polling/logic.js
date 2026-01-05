function startLiveMetadataPolling() {
    const endpoint = `${AuraState.serverUrl}live/now-playing`;

    AuraState.liveMetadataTimer = setInterval(async () => {
        try {
            const res = await fetch(endpoint);
            const meta = await res.json();
            renderLiveMetadata(meta);
        } catch (err) {
            console.warn("Live metadata unavailable");
        }
    }, 5000);
}

function startLiveMode(streamUrl) {
    stopLiveMode(); // safety

    AuraState.mode = "live";
    AuraState.liveStreamUrl = streamUrl;

    DOM.audio.src = streamUrl;
    DOM.audio.play().catch(console.warn);

    startLiveMetadataPolling();
}

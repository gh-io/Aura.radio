HAYSTACK_CONNECTIONS = {
    'default': {
        'ENGINE': 'haystack.backends.solr_backend.SolrEngine',
        'URL': '127.0.0.1', # The URL to your Solr core
        # Other optional settings like 'ADMIN_URL', 'TIMEOUT', etc. can also be added
    },
}

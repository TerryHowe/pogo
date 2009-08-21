# Be sure to restart your server when you modify this file.

# Your secret key for verifying cookie session data integrity.
# If you change this key, all old sessions will become invalid!
# Make sure the secret is at least 30 characters and all random, 
# no regular words or you'll be exposed to dictionary attacks.
ActionController::Base.session = {
  :key         => '_beaker_session',
  :secret      => '4eda9709052c00aa210c89f6b49f1a6d969dd3c8d8f540f47fe8e7b6e7ca245019c3c6f7923cf8050452ee3eeb7d05c5da3de8cdc4e734e0d1809cb9e3306e76'
}

# Use the database for sessions instead of the cookie-based default,
# which shouldn't be used to store highly confidential information
# (create the session table with "rake db:sessions:create")
# ActionController::Base.session_store = :active_record_store

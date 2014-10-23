Sequel.migration do
  change do
    create_table :devices do
      primary_key :id
      String :adapter, null: false
      String :address, null: false
      String :device, null: false
      String :device_type, null: false
      String :manufacturer, null: false

      DateTime :last_heartbeat_received_at
      DateTime :last_heartbeat_logged_at
    end
  end
end

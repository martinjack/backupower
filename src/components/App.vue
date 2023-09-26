<template>
  <BContainer fluid>
    <BRow>
      <BCol md="12">
        <h2 class="text-center display-1 logo">Backupower</h2>

        <div v-if="loading" class="loading"></div>
      </BCol>
    </BRow>

    <BRow v-if="Number(settingsDefault.battery_cap)">
      <BCol md="12">
        <h3 class="text-center">
          <BatteryLifeIcon /> Time left battery - {{ batteryLife() }}
        </h3>
      </BCol>
    </BRow>

    <BRow class="justify-content-center">
      <BCol xl="2" md="3" sm="4" cols="12">
        <div class="data-title">
          <PowerIcon />
          <span>Power</span>
        </div>
      </BCol>

      <BCol xl="1" md="1" sm="2" cols="12">
        <BFormCheckbox
          @click="switchState('power')"
          v-model="control.power"
          switch
          size="lg"
        ></BFormCheckbox>
      </BCol>
    </BRow>

    <BRow class="justify-content-center">
      <BCol xl="2" md="3" sm="4" cols="12">
        <div class="data-title">
          <CoolerIcon />
          <span>Coolers</span>
        </div>
      </BCol>

      <BCol xl="1" md="1" sm="2" cols="12">
        <BFormCheckbox
          @click="switchState('coolers')"
          v-model="control.coolers"
          switch
          size="lg"
        ></BFormCheckbox>
      </BCol>
    </BRow>

    <BRow class="justify-content-center">
      <BCol xl="2" md="3" sm="4" cols="12">
        <div class="data-title">
          <BatteryIcon />
          <span>Battery</span>
        </div>
      </BCol>

      <BCol xl="1" md="1" sm="2" cols="12">
        <div
          class="data-units"
          :class="{
            'low-battery': sensors.bv <= 11.6,
            'hlow-battery': sensors.bv <= 10.9,
          }"
        >
          {{ sensors.bv }} V
        </div>
      </BCol>
    </BRow>

    <BRow class="justify-content-center">
      <BCol xl="2" md="3" sm="4" cols="12">
        <div class="data-title">
          <BatteryIcon />
          <span>Battery</span>
        </div>
      </BCol>

      <BCol xl="1" md="1" sm="2" cols="12">
        <div class="data-units">{{ sensors.bc }} A</div>
      </BCol>
    </BRow>

    <BRow class="justify-content-center">
      <BCol xl="2" md="3" sm="4" cols="12">
        <div class="data-title">
          <ACIcon />
          <span>AC</span>
        </div>
      </BCol>

      <BCol xl="1" md="1" sm="2" cols="12">
        <div class="data-units">{{ sensors.acv }} V</div>
      </BCol>
    </BRow>

    <BRow class="justify-content-center">
      <BCol xl="2" md="3" sm="4" cols="12">
        <div class="data-title">
          <ACIcon />
          <span>AC</span>
        </div>
      </BCol>

      <BCol xl="1" md="1" sm="2" cols="12">
        <div class="data-units">{{ sensors.acc }} A</div>
      </BCol>
    </BRow>

    <BRow class="justify-content-center">
      <BCol xl="2" md="3" sm="4" cols="12">
        <div class="data-title">
          <TemperatureIcon />
          <span>TransAll</span>
        </div>
      </BCol>

      <BCol xl="1" md="1" sm="2" cols="12">
        <div class="data-units">{{ sensors.transall }} °C</div>
      </BCol>
    </BRow>

    <BRow class="justify-content-center">
      <BCol xl="2" md="3" sm="4" cols="12">
        <div class="data-title">
          <TemperatureIcon />
          <span>Trans1</span>
        </div>
      </BCol>

      <BCol xl="1" md="1" sm="2" cols="12">
        <div class="data-units">{{ sensors.trans1 }} °C</div>
      </BCol>
    </BRow>

    <BRow class="justify-content-center">
      <BCol xl="2" md="3" sm="4" cols="12">
        <div class="data-title">
          <TemperatureIcon />
          <span>Trans2</span>
        </div>
      </BCol>

      <BCol xl="1" md="1" sm="2" cols="12">
        <div class="data-units">{{ sensors.trans2 }} °C</div>
      </BCol>
    </BRow>

    <BRow class="justify-content-center">
      <BCol xl="2" md="3" sm="4" cols="12">
        <div class="data-title">
          <TemperatureIcon />
          <span>TransKeys</span>
        </div>
      </BCol>

      <BCol xl="1" md="1" sm="2" cols="12">
        <div class="data-units">{{ sensors.transkeys }} °C</div>
      </BCol>
    </BRow>

    <BRow class="mt-3 justify-content-center">
      <BCol md="12">
        <h2 class="text-center settings display-3">
          <SettingsIcon />
          Settings
        </h2>
      </BCol>

      <BCol md="6" sm="8" xl="4" class="mb-3">
        <BCard no-body>
          <BCardHeader header-tag="nav">
            <BNav card-header tabs>
              <BNavItem
                @click="openTab('wifi')"
                :active="settingsTab.wifi"
                href="javascript:void(0)"
                :class="{
                  'text-white': !settingsTab.wifi,
                }"
              >
                Wifi
              </BNavItem>

              <BNavItem
                @click="openTab('default')"
                :active="settingsTab.default"
                href="javascript:void(0)"
                :class="{
                  'text-white': !settingsTab.default,
                }"
              >
                Default parameters
              </BNavItem>

              <BNavItem
                @click="openTab('factory')"
                :active="settingsTab.factory"
                href="javascript:void(0)"
                :class="{
                  'text-white': !settingsTab.factory,
                }"
              >
                Factory
              </BNavItem>
            </BNav>
          </BCardHeader>

          <BCardBody>
            <BOverlay :show="settingsLoading" variant="dark" />

            <div v-if="settingsTab.wifi">
              <BFormGroup label="Name SSID">
                <BFormSelect
                  v-model="v$.settingsWifi.wifi_name.$model"
                  :options="wifiList"
                >
                  <template #first>
                    <BFormSelectOption :value="null" disabled>
                      -- Please select SSID --
                    </BFormSelectOption>
                  </template>
                </BFormSelect>
              </BFormGroup>

              <BFormGroup label="Password" class="mt-2">
                <BFormInput
                  v-model="v$.settingsWifi.wifi_password.$model"
                  type="password"
                  placeholder="Please enter password"
                  :disabled="v$.settingsWifi.wifi_name.$invalid"
                  :state="
                    v$.settingsWifi.wifi_password.$model
                      ? v$.settingsWifi.wifi_password.$invalid
                        ? false
                        : true
                      : null
                  "
                />
              </BFormGroup>

              <BButtonGroup class="mt-3 d-flex">
                <BButton @click="searchWifi">Search</BButton>

                <BButton
                  @click="updateWifi"
                  variant="success"
                  :disabled="v$.settingsWifi.$invalid"
                >
                  Save
                </BButton>
              </BButtonGroup>
            </div>

            <div v-if="settingsTab.default">
              <BRow class="mt-3">
                <BCol cols="6" class="d-flex align-items-center">AP name</BCol>

                <BCol cols="6">
                  <BFormInput
                    v-model="v$.settingsDefault.ap_name.$model"
                    :state="
                      v$.settingsDefault.ap_name.$model
                        ? v$.settingsDefault.ap_name.$invalid
                          ? false
                          : true
                        : null
                    "
                  />
                </BCol>
              </BRow>

              <BRow class="mt-3">
                <BCol cols="6" class="d-flex align-items-center">
                  AP password
                </BCol>

                <BCol cols="6">
                  <BFormInput
                    v-model="v$.settingsDefault.ap_password.$model"
                    type="password"
                    :state="
                      v$.settingsDefault.ap_password.$model
                        ? v$.settingsDefault.ap_password.$invalid
                          ? false
                          : true
                        : null
                    "
                  />
                </BCol>
              </BRow>

              <BRow class="mt-3">
                <BCol cols="6">Power (AutoStart)</BCol>

                <BCol cols="6">
                  <BFormCheckbox v-model="v$.settingsDefault.power.$model">
                    {{ powerTittle }}
                  </BFormCheckbox>
                </BCol>
              </BRow>

              <BRow class="mt-3">
                <BCol cols="6">Coolers (AutoStart)</BCol>

                <BCol cols="6">
                  <BFormCheckbox v-model="v$.settingsDefault.coolers.$model">
                    {{ coolersTittle }}
                  </BFormCheckbox>
                </BCol>
              </BRow>

              <BRow class="mt-3">
                <BCol cols="6" class="d-flex align-items-center">
                  MinTemp ℃
                </BCol>

                <BCol cols="6">
                  <BInputGroup>
                    <template #append>
                      <BInputGroupText>℃</BInputGroupText>
                    </template>

                    <BFormInput
                      v-model="v$.settingsDefault.mintemp.$model"
                      type="number"
                      :state="
                        v$.settingsDefault.mintemp.$model
                          ? v$.settingsDefault.mintemp.$invalid
                            ? false
                            : true
                          : null
                      "
                    />
                  </BInputGroup>
                </BCol>
              </BRow>

              <BRow class="mt-3">
                <BCol cols="6" class="d-flex align-items-center">
                  MaxTemp ℃
                </BCol>

                <BCol cols="6">
                  <BInputGroup>
                    <template #append>
                      <BInputGroupText>℃</BInputGroupText>
                    </template>

                    <BFormInput
                      v-model="v$.settingsDefault.maxtemp.$model"
                      type="number"
                      :state="
                        v$.settingsDefault.maxtemp.$model
                          ? v$.settingsDefault.maxtemp.$invalid
                            ? false
                            : true
                          : null
                      "
                    />
                  </BInputGroup>
                </BCol>
              </BRow>

              <BRow class="mt-3">
                <BCol cols="6" class="d-flex align-items-center">
                  Battery capacity
                </BCol>

                <BCol cols="6">
                  <BInputGroup>
                    <template #append>
                      <BInputGroupText>Ah</BInputGroupText>
                    </template>

                    <BFormInput
                      v-model="v$.settingsDefault.battery_cap.$model"
                      type="number"
                      :state="
                        v$.settingsDefault.battery_cap.$model
                          ? v$.settingsDefault.battery_cap.$invalid
                            ? false
                            : true
                          : null
                      "
                    />
                  </BInputGroup>
                </BCol>
              </BRow>

              <BRow class="mt-3">
                <BCol cols="12">
                  <BButtonGroup class="d-flex">
                    <BButton
                      @click="updateSettings"
                      variant="success"
                      :disabled="v$.settingsDefault.$invalid"
                    >
                      Update
                    </BButton>
                  </BButtonGroup>
                </BCol>
              </BRow>
            </div>

            <div v-if="settingsTab.factory" class="text-center">
              <p class="font-weight-bold">Warning!</p>

              <BButton @click="factoryReset" variant="danger">
                Factory reset settings
              </BButton>
            </div>
          </BCardBody>
        </BCard>
      </BCol>
    </BRow>

    <BModal
      v-if="modalReset"
      v-model="modal"
      centered
      bodyClass="text-center"
      hideFooter
      hideHeader
      noCloseOnBackdrop
      noCloseOnEsc
    >
      <b>Restart device {{ timeReset }} seconds ....</b>

      <BProgress
        class="mt-3"
        :value="timeReset"
        :max="10"
        height="10px"
        striped
        animated
      />
    </BModal>

    <BRow>
      <BCol md="12" class="text-center version">v{{ appVersion }}</BCol>
    </BRow>
  </BContainer>
</template>
<script>
// PowerIcon
import PowerIcon from "icons/PowerSocketDe.vue";
// CoolerIcon
import CoolerIcon from "icons/Fan.vue";
// BatteryIcon
import BatteryIcon from "icons/CarBattery.vue";
// ACIcon
import ACIcon from "icons/Flash.vue";
// TemperatureIcon
import TemperatureIcon from "icons/Thermometer.vue";
// SettingsIcon
import SettingsIcon from "icons/Wrench.vue";
// BatteryLifeIcon
import BatteryLifeIcon from "icons/BatteryClock.vue";
// Vuelidate
import useVuelidate from "@vuelidate/core";
import {
  required,
  minLength,
  maxLength,
  requiredIf,
  numeric,
  between,
} from "@vuelidate/validators";
//

const allowedCharacters = (value) => {
  return value.match(/^[а-яА-ЯіІїЇґҐa-zA-Z0-9() ]+$/);
};

export default {
  /**
   *
   * NAME
   *
   */
  name: "App",
  /**
   *
   * COMPONENTS
   *
   */
  components: {
    PowerIcon,
    CoolerIcon,
    BatteryIcon,
    ACIcon,
    TemperatureIcon,
    SettingsIcon,
    BatteryLifeIcon,
  },
  /**
   *
   * SETUP
   *
   */
  setup() {
    const appVersion = import.meta.env.VERSION;

    return {
      v$: useVuelidate(),
      appVersion,
    };
  },
  /**
   *
   * DATA
   *
   */
  data() {
    return {
      loading: false,
      settingsLoading: false,
      control: {
        power: false,
        coolers: false,
      },
      sensors: {
        bv: "0.0",
        bc: "0.0",
        acv: "0.0",
        acc: "0.0",
        transall: "0.0",
        trans1: "0.0",
        trans2: "0.0",
        transkeys: "0.0",
      },
      settingsWifi: {
        wifi_name: null,
        wifi_password: null,
      },
      settingsDefault: {
        ap_name: null,
        ap_password: null,
        power: false,
        coolers: false,
        mintemp: 0,
        maxtemp: 0,
        battery_cap: 0,
      },
      settingsTab: {
        wifi: true,
        default: false,
        factory: false,
      },
      powerTittle: "OFF",
      coolersTittle: "OFF",
      wifiList: [],
      wifi: null,
      modal: false,
      modalReset: false,
      timeReset: 10,
      refreshData: null,
      reset: false,
      resetInterval: null,
    };
  },
  /**
   *
   * WATCH
   *
   */
  watch: {
    "settingsDefault.power": function (state) {
      if (state) {
        this.powerTittle = "ON";
      } else {
        this.powerTittle = "OFF";
      }
    },
    "settingsDefault.coolers": function (state) {
      if (state) {
        this.coolersTittle = "ON";
      } else {
        this.coolersTittle = "OFF";
      }
    },
    modalReset: function (state) {
      if (state) {
        this.modal = this.modalReset;

        this.resetInterval = setInterval(() => {
          if (this.timeReset <= 0) {
            clearInterval(this.resetInterval);

            let wifi = this.settingsWifi.wifi_name;

            if (this.reset || !wifi) wifi = this.settingsDefault.ap_name;

            toast.info(`Please select wifi in settings (${wifi})`, {
              autoClose: 10000,
            });

            this.modalReset = false;
            this.reset = false;
          } else {
            this.timeReset -= 1;
          }
        }, 1000);
      } else {
        this.timeReset = 10;

        clearInterval(this.resetInterval);
      }
    },
  },
  /**
   *
   * VALIDATIONS
   *
   */
  validations() {
    return {
      settingsWifi: {
        wifi_name: {
          required: requiredIf(this.settingsTab.wifi),
          minLength: minLength(3),
          maxLength: maxLength(50),
          allowedCharacters,
        },
        wifi_password: {
          required: requiredIf(this.settingsTab.wifi),
          minLength: minLength(8),
          maxLength: maxLength(50),
        },
      },
      settingsDefault: {
        ap_name: {
          required,
          minLength: minLength(8),
          maxLength: maxLength(50),
          allowedCharacters,
        },
        ap_password: {
          required,
          minLength: minLength(8),
          maxLength: maxLength(50),
        },
        power: { required },
        coolers: { required },
        mintemp: {
          required,
          maxLength: maxLength(2),
          numeric,
          between: between(30, 60),
        },
        maxtemp: {
          required,
          maxLength: maxLength(2),
          numeric,
          between: between(30, 60),
        },
        battery_cap: {
          required,
          maxLength: maxLength(4),
          numeric,
          between: between(0, 1000),
        },
      },
    };
  },
  /**
   *
   * CREATED
   *
   */
  created() {
    this.getData();
    this.getSettings();
  },
  /**
   *
   * MOUNTED
   *
   */
  mounted() {
    this.initData();
  },
  /**
   *
   * METHODS
   *
   */
  methods: {
    /**
     *
     * INIT DATA
     *
     * @param {bool} state
     *
     */
    initData(state = true) {
      if (state) {
        this.refreshData = setInterval(() => {
          this.getData();
        }, 2000);
      } else {
        clearInterval(this.refreshData);
      }
    },
    /**
     *
     * GET DATA
     *
     */
    getData() {
      this.loading = true;

      axios.get("./data").then((response) => {
        this.loading = false;

        if (response.data.status) {
          let data = response.data;

          this.control.power = data.power;
          this.control.coolers = data.coolers;

          this.sensors.bv = data.bv;
          this.sensors.bc = Math.abs(data.bc);
          this.sensors.acv = data.acv;
          this.sensors.acc = Math.abs(data.acc);
          this.sensors.transall = data.transall;
          this.sensors.trans1 = data.trans1;
          this.sensors.trans2 = data.trans2;
          this.sensors.transkeys = data.transkeys;
        }
      });
    },
    /**
     *
     * GET SETTINGS
     *
     */
    getSettings() {
      this.loading = false;

      axios
        .post("./settings", { type: "get" })
        .then((response) => {
          this.loading = false;

          if (response.data.status) {
            for (let item in this.settingsWifi)
              this.settingsWifi[item] = response.data[item];

            for (let item in this.settingsDefault)
              this.settingsDefault[item] = response.data[item];

            if (!this.settingsWifi.wifi_name) {
              this.settingsWifi.wifi_name = null;
            } else {
              this.wifiList = [
                {
                  text: this.settingsWifi.wifi_name,
                  value: this.settingsWifi.wifi_name,
                },
              ];
            }
          }
        })
        .catch(() => {
          this.loading = false;
        });
    },
    /**
     *
     * UPDATE SETTINGS
     *
     */
    updateSettings() {
      this.v$.settingsDefault.$validate().then((result) => {
        if (result) {
          this.settingsLoading = true;

          this.initData(false);

          axios
            .post("./settings", { type: "update", ...this.settingsDefault })
            .then((response) => {
              this.settingsLoading = false;

              if (response.data.status) {
                toast.success("Settings updated");
              } else {
                toast.error("Error update settings");
              }

              this.initData();
            })
            .catch(() => {
              this.settingsLoading = false;

              toast.error("Error update settings");

              this.initData();
            });
        }
      });
    },
    /**
     *
     * UPDATE WIFI
     *
     */
    updateWifi() {
      this.v$.settingsWifi.$validate().then((result) => {
        if (result) {
          this.settingsLoading = true;

          axios
            .post("./settings", { type: "wifi_update", ...this.settingsWifi })
            .then((response) => {
              this.settingsLoading = false;

              if (response.data.status) {
                toast.success("Wifi updated");

                this.modalReset = true;
              } else {
                toast.error("Error wifi update");
              }
            })
            .catch(() => {
              this.settingsLoading = false;

              toast.error("Error wifi update");
            });
        }
      });
    },
    /**
     *
     * FACTORY RESET
     *
     */
    factoryReset() {
      this.initData(false);

      axios
        .post("./settings", { type: "factory_reset" })
        .then((response) => {
          this.settingsLoading = false;

          this.modalReset = true;
          this.reset = true;
        })
        .catch(() => {
          this.settingsLoading = false;
        });
    },
    /**
     *
     * SEARCH WIFI
     *
     */
    searchWifi() {
      this.settingsLoading = true;

      this.initData(false);

      axios
        .post("./settings", { type: "wifi" })
        .then((response) => {
          this.settingsLoading = false;

          if (response.data.status) {
            this.wifiList = response.data.list.map((item) => {
              return {
                text: item.name + "(" + item.type + ")",
                value: item.name,
              };
            });

            for (let item in this.settingsWifi) this.settingsWifi[item] = null;

            toast.success("Wifi found");

            this.initData();
          }
        })
        .catch(() => {
          this.settingsLoading = false;

          toast.error("Error search wifi");

          this.initData();
        });
    },
    /**
     *
     * OPEN TAB
     *
     * @param {string} tab
     *
     */
    openTab(tab) {
      for (let item in this.settingsTab) this.settingsTab[item] = false;

      this.settingsTab[tab] = true;
    },
    /**
     *
     * SWITCH STATE
     *
     * @param {string} name
     *
     */
    switchState(name) {
      this.loading = true;

      axios
        .get("./relay?name=" + name + "&state=" + Number(!this.control[name]))
        .then(() => {
          this.loading = false;
        })
        .catch(() => {
          this.loading = false;
        });
    },
    /**
     *
     * BATTERY LIFE
     *
     */
    batteryLife() {
      let time =
          (this.settingsDefault.battery_cap /
            Math.abs(parseFloat(this.sensors.bc))) *
          (1 - 0.2),
        hour = Math.floor(time),
        minutes = Math.round((time - hour) * 60);

      if (hour) {
        return `${hour} h ${minutes} m`;
      } else {
        return `${minutes} m`;
      }
    },
  },
};
</script>

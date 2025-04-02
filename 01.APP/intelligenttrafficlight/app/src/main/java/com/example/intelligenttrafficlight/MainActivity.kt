package com.example.intelligenttrafficlight

import android.annotation.SuppressLint
import android.content.Intent
import android.content.SharedPreferences
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.CountDownTimer
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.widget.ArrayAdapter
import androidx.appcompat.app.AlertDialog
import com.blankj.utilcode.util.GsonUtils
import com.blankj.utilcode.util.LogUtils
import com.example.intelligenttrafficlight.Utils.HandlerAction
import com.example.intelligenttrafficlight.Utils.MToast
import com.example.intelligenttrafficlight.databinding.ActivityMainBinding
import com.example.intelligentwaterheater.Model.DataDTO
import com.example.intelligentwaterheater.Model.Send
import com.example.intelligentwaterheater.Utils.Common
import com.example.intelligentwaterheater.Utils.DeviceIsOnline
import com.example.intelligentwaterheater.Utils.TimeCycle
import com.example.intelligentwindows.Model.Receive
import com.example.intelligentwindows.Utils.ViewSetIsClick
import com.google.gson.Gson
import com.gyf.immersionbar.ImmersionBar
import com.itfitness.mqttlibrary.MQTTHelper
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken
import org.eclipse.paho.client.mqttv3.MqttCallback
import org.eclipse.paho.client.mqttv3.MqttMessage
import org.json.JSONObject

class MainActivity : AppCompatActivity(), HandlerAction {
    private lateinit var binding: ActivityMainBinding
    private var arrayList = ArrayList<String>()// debug消息数据
    private var adapter: ArrayAdapter<String>? = null
    private var isDebugView = false //是否显示debug界面
    private lateinit var sharedPreferences: SharedPreferences
    private lateinit var editor: SharedPreferences.Editor
    private var loginTimer: CountDownTimer? = null//协成定时器
    private var onlineFlag = false //是否在线标识
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        mqttServer()
        initView()
        isOnline()
    }

    private fun initView() {
        setSupportActionBar(findViewById(R.id.toolbar))
        binding.toolbarLayout.title = title
        ImmersionBar.with(this).init()
        adapter = ArrayAdapter(this, android.R.layout.simple_list_item_1, arrayList)
        binding.debugView.adapter = adapter

        sharedPreferences = getSharedPreferences("light", MODE_PRIVATE)
        editor = sharedPreferences.edit()
        val red = sharedPreferences.getString("r", null)
        val green = sharedPreferences.getString("g", null)
        if (red != null) {
            binding.remainsRedTime1.text = red
            binding.remainsRedTime2.text = red
        }
        if (green != null) {
            binding.remainsGreenTime1.text = green
            binding.remainsGreenTime2.text = green
        }
        setViewOnClick()
    }

    private fun setViewOnClick() {
        // 创建数据
        val items = arrayOf("15", "30", "45", "60", "75", "90")
        binding.remainsRedTime1.setOnClickListener {
            if (Common.mqttHelper != null)
                if (Common.mqttHelper?.subscription == true) {
                    val builder = AlertDialog.Builder(this)
                    builder.setTitle("设置时间").setItems(items) { _, index ->
                        binding.remainsRedTime1.text = items[index]
                        binding.remainsRedTime2.text = items[index]
                        sendMsg(
                            Gson().toJson(
                                Send(
                                    1, DataDTO(
                                        r = (index + 1) * 15,
                                        g = binding.remainsGreenTime1.text.toString().toInt(),
                                    )
                                )
                            )
                        )
                        editor.putString("r", "${(index + 1) * 15}")
                        // 提交数据
                        editor.commit()
                        builder.create().dismiss()
                    }
                    builder.create().show()
                } else {
                    MToast.mToast(this, "未建立连接")
                }
            else MToast.mToast(this, "未建立连接")
        }
        binding.remainsRedTime2.setOnClickListener {
            if (Common.mqttHelper != null)
                if (Common.mqttHelper?.subscription == true) {
                    val builder = AlertDialog.Builder(this)
                    builder.setTitle("设置时间").setItems(items) { _, index ->
                        binding.remainsRedTime1.text = items[index]
                        binding.remainsRedTime2.text = items[index]
                        sendMsg(
                            Gson().toJson(
                                Send(
                                    1, DataDTO(
                                        r = (index + 1) * 15,
                                        g = binding.remainsGreenTime2.text.toString().toInt()
                                    )
                                )
                            )
                        )
                        editor.putString("r", "${(index + 1) * 15}")
                        // 提交数据
                        editor.commit()
                        builder.create().dismiss()
                    }
                    builder.create().show()
                } else {
                    MToast.mToast(this, "未建立连接")
                }
            else MToast.mToast(this, "未建立连接")
        }
        binding.remainsGreenTime1.setOnClickListener {
            if (Common.mqttHelper != null)
                if (Common.mqttHelper?.subscription == true) {
                    val builder = AlertDialog.Builder(this)
                    builder.setTitle("设置时间").setItems(items) { _, index ->
                        binding.remainsGreenTime1.text = items[index]
                        binding.remainsGreenTime2.text = items[index]
                        sendMsg(
                            Gson().toJson(
                                Send(
                                    1, DataDTO(
                                        r = binding.remainsRedTime1.text.toString().toInt(),
                                        g = (index + 1) * 15
                                    )
                                )
                            )
                        )
                        editor.putString("g", "${(index + 1) * 15}")
                        // 提交数据
                        editor.commit()
                        builder.create().dismiss()
                    }
                    builder.create().show()
                } else {
                    MToast.mToast(this, "未建立连接")
                }
            else MToast.mToast(this, "未建立连接")
        }
        binding.remainsGreenTime2.setOnClickListener {
            if (Common.mqttHelper != null)
                if (Common.mqttHelper?.subscription == true) {
                    val builder = AlertDialog.Builder(this)
                    builder.setTitle("设置时间").setItems(items) { _, index ->
                        binding.remainsGreenTime1.text = items[index]
                        binding.remainsGreenTime2.text = items[index]
                        sendMsg(
                            Gson().toJson(
                                Send(
                                    1, DataDTO(
                                        r = binding.remainsRedTime1.text.toString().toInt(),
                                        g = (index + 1) * 15
                                    )
                                )
                            )
                        )
                        editor.putString("g", "${(index + 1) * 15}")
                        // 提交数据
                        editor.commit()
                        builder.create().dismiss()
                    }
                    builder.create().show()
                } else {
                    MToast.mToast(this, "未建立连接")
                }
            else MToast.mToast(this, "未建立连接")
        }
    }

    private fun mqttServer() {
        Common.mqttHelper =
            MQTTHelper(this, Common.Sever, Common.DriveID, Common.DriveName, Common.DrivePassword,true, 30, 60)
        Common.mqttHelper!!.connect(Common.ReceiveTopic, 1, true, object : MqttCallback {
            override fun connectionLost(cause: Throwable?) {
//                MToast.mToast(this@MainActivity,"连接超时")
            }

            override fun messageArrived(topic: String?, message: MqttMessage?) {
                //收到消息
                val data = Gson().fromJson(message.toString(), Receive::class.java)
                LogUtils.eTag("接收到消息", message?.payload?.let { String(it) })
                debugViewData(2, message?.payload?.let { String(it) }!!)
                println(data)
                onlineFlag = true
                analysisOfData(data)
            }

            override fun deliveryComplete(token: IMqttDeliveryToken?) {
            }
        })

    }

    @SuppressLint("ResourceType")
    private fun analysisOfData(data: Receive) {
        if (data.light1 != null) {
            setAllSelect(1, false)
            setAllSelect(2, false)
            when (data.light1) {
                "0" -> {
                    binding.redSelect1.isSelected = true
                    binding.greenSelect2.isSelected = true
                }
                "1" -> {
                    binding.yellowSelect1.isSelected = true
                    binding.yellowSelect2.isSelected = true
                }
                "2" -> {
                    binding.greenSelect1.isSelected = true
                    binding.redSelect2.isSelected = true
                }
            }
        }
        if (data.G_Time != null) {
            binding.remainsGreenTime2.text = data.G_Time
            binding.remainsGreenTime1.text = data.G_Time
            editor.putString("g", "${data.G_Time}")
            // 提交数据
            editor.commit()
        }
        if (data.car != null) {
            binding.car.text = data.car

        }
        if (data.R_Time != null) {
            binding.remainsRedTime1.text = data.R_Time
            binding.remainsRedTime2.text = data.R_Time
            editor.putString("r", "${data.R_Time}")
            // 提交数据
            editor.commit()
        }
    }

    private fun setAllSelect(index: Int, b: Boolean) {
     
    }
    /***
     * 判断硬件是否在线
     */
    private fun isOnline() {
        loginTimer = object : CountDownTimer(15000, 5000) {
            //18000ms运行一次onTick里面的方法
            override fun onTick(p0: Long) {
                Log.e("定时器测试:", "onlineFlag:${onlineFlag}")
                runOnUiThread {
                    binding.online.text = if (onlineFlag) "在线" else "离线"
                }
                onlineFlag = false

            }

            override fun onFinish() {
                isOnline()
            }
        }.start()
    }
    /**
     * 发送消息
     */
    private fun sendMsg(data: String) {

        debugViewData(1, data)
        Thread {
            for (i in 0..1) {
                Thread.sleep(100)
                Common.mqttHelper!!.publish(Common.PushTopic, data, 1)
            }
        }.start()
    }

    /**
     * 去除实体类中的为null的
     */
    private fun modelRemoveNull(send: String): String {
        val jsonObject = JSONObject(send)
        val dataObject = jsonObject.getJSONObject("data")
        val filteredDataObject = JSONObject()
        val keys = dataObject.keys()
        while (keys.hasNext()) {
            val key = keys.next()
            val value = dataObject.opt(key)
            if (value != null && value != JSONObject.NULL) {
                filteredDataObject.put(key, value)
            }
        }
        val filteredJsonObject =
            JSONObject().put("cmd", jsonObject["cmd"]).put("data", filteredDataObject)
        return filteredJsonObject.toString()
    }

    /***
     * 消息面板
     * str 类型：1 本机发送的消息  2：接收的消息
     */
    private fun debugViewData(str: Int, data: String) {
       
        adapter?.notifyDataSetChanged()
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        menuInflater.inflate(R.menu.menu_scrolling, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {

        return when (item.itemId) {
            R.id.setDebugView -> {
                isDebugView = !isDebugView
                binding.debugView.visibility = if (isDebugView) View.VISIBLE else View.GONE
                true
            }
            else -> super.onOptionsItemSelected(item)
        }
    }
}